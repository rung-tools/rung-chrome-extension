type notifications = {.
    "totalUnread": int,
    "edges": array({.
        "cursor": string,
        "node": {.
            "id": string,
            "type_": string,
            "text": string,
            "fields": {.
                "app": option(string),
                "card": option(string),
                "comment": option(string),
                "count": option(int),
                "followers": option(array(string)),
                "responsible": option(string)
            },
            "read": Js.boolean,
            "date": string
        }
    }),
    "pageInfo": {.
        "endCursor": option(string),
        "hasNextPage": Js.boolean
    }
};

type state = {
    notifications: notifications,
    loading: bool,
    boxRef: ref(option(Dom.element))
};

let initialState = () => {
    notifications: {
        "totalUnread": 0,
        "edges": [||],
        "pageInfo": {
            "endCursor": None,
            "hasNextPage": Js.false_
        }
    },
    loading: true,
    boxRef: ref(None)
};

type action('a) =
    | LoadNotifications
    | NotificationsLoaded('a)
    | ReadNotification(string);

let component = ReasonReact.reducerComponent("NotificationList");

module NotificationsQuery = [%graphql {|
query($first: Int, $after: String) {
    notifications(first: $first, after: $after) {
        totalUnread
        edges {
            cursor
            node {
                id
                type_: type
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
    | LoadNotifications => ReasonReact.SideEffects((self) => {
        let open Js.Promise;
        let query = NotificationsQuery.make(~first=5, ());
        Request.sendQuery(query)
        |> then_((response) => resolve(self.reduce((_) => NotificationsLoaded(response##notifications), ())))
        |> ignore
    })
    | NotificationsLoaded(notifications) => ReasonReact.Update({...state, loading: false, notifications})
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
let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    didUpdate: ({oldSelf, newSelf}) => {
        let totalUnread = newSelf.state.notifications##totalUnread;
        if (oldSelf.state.notifications##totalUnread != totalUnread) {
            Chrome.chrome##browserAction##setBadgeText({"text": string_of_int(totalUnread)})
        }
    },
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
            <div className="custom-scrollbar" style=(Style.notifications)>
            {
                switch (Array.length(notifications##edges), loading) {
                | (0, false) => <div style=(Style.nothing)>(show(t("nothing")))</div>
                | _ => notifications##edges
                |> Js.Array.map((edge) => {
                    <Notification
                        key=edge##cursor
                        type_=edge##node##type_
                        text=edge##node##text
                        read=Js.to_bool(edge##node##read)
                        date=edge##node##date
                        fields=edge##node##fields
                    />
                })
                |> (elements) =>
                    <ul className="collection">
                        (ReasonReact.arrayToElement(elements))
                    </ul>
                }
            }
            </div>
        </div>
}
