type notification = {.
    "id": string,
    "dispatcher": Js.Nullable.t(string),
    "_type": string,
    "date": string,
    "task": Js.Nullable.t(string),
    "readDate": Js.Nullable.t(string),
    "name": string,
    "values": Js.Nullable.t(array(string))
};

type action =
    | LoadNotifications
    | SetNotifications(array(notification))
    | ReadNotification(string);

type state = {
    notifications: array(notification),
    loading: bool,
    boxRef: ref(option(Dom.element))
};

[@bs.val]
external parseNotifications : string => array(notification) = "JSON.parse";

let component = ReasonReact.reducerComponent("NotificationList");

let initialState = () => {
    notifications: [||],
    loading: true,
    boxRef: ref(None)
};

module NotificationsQuery = [%graphql {|
{
    notifications(first: $first, after: $after) {
        totalUnread
        edges {
            cursor
            node {
                id
                type
                text
                fields {
                    count
                    app
                    responsible
                    card
                    comment
                    followers
                }
                read
                date
            }
        }
        pageInfo {
            hasNextPage
            endCursor
        }
    }
}
|}];

let reducer = (action, state) =>
    switch action {
    | LoadNotifications => ReasonReact.SideEffects((self) => Js.Promise.(
        Request.request("/notifications")
        |> then_((result) => parseNotifications(result)
            |> (notifications) => {
                let validNotifications = notifications
                |> Js.Array.filter((notification) => String.lowercase(notification##_type) == notification##_type);

                Chrome.(
                    chrome##browserAction##setBadgeText({
                        "text": validNotifications
                        |> Js.Array.filter((notification) => Js.Nullable.test(notification##readDate))
                        |> Array.length
                        |> string_of_int
                    }));
                self.reduce((_) => SetNotifications(validNotifications), ())
            }
            |> resolve))
        |> ignore)
    | SetNotifications(notifications) => ReasonReact.Update({...state, loading: false, notifications})
    | ReadNotification(id) => ReasonReact.SideEffects((self) => Js.Promise.(
        Request.request("/notifications/" ++ id, ~method_=Fetch.Put)
        |> then_((_) => resolve(self.reduce((_) => LoadNotifications, ()))))
        |> ignore)
    };

module Style = {
    let container = ReactDOMRe.Style.make(
        ~padding="12px",
        ~fontWeight="100", ());
    let notifications = ReactDOMRe.Style.make(
        ~maxHeight="324px",
        ~overflow="auto",
        ~borderTop="1px solid #BDBDBD", ());
    let nothing = ReactDOMRe.Style.make(
        ~padding="64px 0",
        ~textAlign="center",
        ~fontSize="16px",
        ~color="#9E9E9E", ());
    let header = ReactDOMRe.Style.make(
        ~lineHeight="48px",
        ~paddingLeft="12px",
        ~fontSize="14px",
        ~color="rgba(0, 0, 0, 0.54)", ())
};

let t = key => Chrome.(chrome##i18n##getMessage(key));

let replace = Js.String.replace;

let countAlerts = notification =>
    switch (Js.Nullable.to_opt(notification##values)) {
    | Some(result) => Array.length(result)
    | None => 0
    };

let getNotificationStyles = (notification) =>
    switch (notification##_type) {
    | "alerts-created" => ("list", "green",
        t("alertsCreated")
        |> replace("{{COUNT}}", string_of_int(countAlerts(notification)))
        |> replace("{{APP}}", notification##name))
    | "alerts-updated" => ("system_update_alt", "teal",
        t("alertsUpdated")
        |> replace("{{COUNT}}", string_of_int(countAlerts(notification)))
        |> replace("{{APP}}", notification##name))
    | "alerts-deleted" => ("delete", "red",
        t("alertsDeleted")
        |> replace("{{COUNT}}", string_of_int(countAlerts(notification)))
        |> replace("{{APP}}", notification##name))
    | "permissions-updated" => ("security", "pink", t("permissionsUpdated"))
    | "task-created" => ("warning", "red", t("taskCreated"))
    | "alert-comment" => ("comment", "indigo", t("alertComment"))
    | "alert-follow" => ("people", "brown", t("alertFollow"))
    | "alert-unfollow" => ("do_not_disturb", "red", t("alertUnfollow"))
    | "alert-manually-deleted" => ("delete_forever", "red",
        t("alertDeleted")
        |> replace("{{TITLE}}", notification##name))
    | _ => ("alarm", "red", "")
    };

let (>>=) = Js.Nullable.bind;

let openTab = (kind, suffix) =>
    Chrome.(chrome##tabs##create({"url": "https://app.rung.com.br/" ++ kind ++ "/" ++ suffix}));

let handleClickNotification = (notification) =>
    switch (notification##_type) {
    | "alerts-created" | "alerts-updated" =>
        notification##values >>= ([@bs] (values) => values
            |> Js.Array.joinWith(";")
            |> openTab("n"))
        |> ignore
    | "permissions-updated" | "alert-manually-deleted" | "alerts-deleted" => ()
    | "alert-comment" | "alert-follow" | "alert-unfollow" | "task-created" =>
        notification##dispatcher >>= ([@bs] (dispatcher) => openTab("i", dispatcher))
        |> ignore
    | _ =>
        notification##task >>= ([@bs] (task) => openTab("i", task))
        |> ignore
    };

let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    didMount: (self) => {
        self.reduce((_) => LoadNotifications, ());
        ReasonReact.NoUpdate
    },
    render: ({state: {loading, notifications}, handle, reduce}) =>
        <div style=(Style.container)>
            <div style=(Style.header)>
                (show(t("notifications")))
            </div>
            <LinearLoading loading />
            <div
                className="custom-scrollbar"
                style=(Style.notifications)>
            {
                switch (Array.length(notifications), loading) {
                | (0, false) => <div style=(Style.nothing)>(show(t("nothing")))</div>
                | _ => notifications
                |> Js.Array.map(notification => {
                    let (icon, color, text) = getNotificationStyles(notification);

                    <Notification
                        key=(notification##id)
                        text
                        color
                        icon
                        onClick=((_) => {
                            if (Js.Nullable.test(notification##readDate)) {
                                reduce((_) => ReadNotification(notification##id), ())
                            };

                            handle((_event, _self) => handleClickNotification(notification), ())
                        })
                        date=(notification##date)
                        read=(!Js.Nullable.test(notification##readDate))
                    />
                })
                |> ReasonReact.arrayToElement
                |> (elements) =>
                    <ul className="collection">
                        (elements)
                    </ul>
                }
            }
            </div>
        </div>
}
