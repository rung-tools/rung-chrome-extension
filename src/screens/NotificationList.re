type notification = {.
    "id": string,
    "dispatcher": string,
    "_type": string,
    "date": string,
    "task": string,
    "name": string,
    "values": option(array(string))
};

type action =
    | LoadNotifications
    | SetNotifications(array(notification));

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

let reducer = (action, state) =>
    switch action {
    | LoadNotifications => ReasonReact.SideEffects((self) => Js.Promise.(
        Request.request("/notifications")
        |> then_((result) => parseNotifications(result)
            |> (notifications) => {
                let validNotifications = notifications
                |> Js.Array.filter((notification) => String.lowercase(notification##_type) == notification##_type);

                Chrome.(chrome##browserAction##setBadgeText(
                    {"text": string_of_int(Array.length(validNotifications))}));
                self.reduce((_) => SetNotifications(Js.Array.concat(state.notifications, validNotifications)), ())
            }
            |> resolve))
        |> ignore)
    | SetNotifications(notifications) => ReasonReact.Update({...state, loading: false, notifications})
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
    switch (notification##values) {
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

let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    didMount: (self) => {
        self.reduce((_) => LoadNotifications, ());
        ReasonReact.NoUpdate
    },
    render: ({state: {loading, notifications}}) =>
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
