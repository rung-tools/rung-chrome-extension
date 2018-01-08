type notification = {.
    "id": string,
    "dispatcher": string,
    "type": string,
    "date": string,
    "task": string
};

type action =
    | LoadNotifications(int)
    | SetNotifications(array(notification));

type state = {
    page: int,
    notifications: array(notification),
    loading: bool,
    boxRef: ref(option(Dom.element))
};

[@bs.val]
external parseNotifications : string => array(notification) = "JSON.parse";

let component = ReasonReact.reducerComponent("NotificationList");

let initialState = () => {
    page: 0,
    notifications: [||],
    loading: true,
    boxRef: ref(None)
};

let reducer = (action, state) =>
    switch action {
    | LoadNotifications(_page) => ReasonReact.SideEffects((self) => Js.Promise.(
        Request.request("/notifications")
        |> then_((result) => parseNotifications(result)
            |> (notifications) => self.reduce((_) => SetNotifications(notifications), ())
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

let setBoxRef = (theRef, {ReasonReact.state}) =>
    state.boxRef := Js.Nullable.to_opt(theRef);

let loadMoreNotifications = (_self) => SetNotifications([||]);

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    didMount: (self) => {
        self.reduce((_) => LoadNotifications(0), ());
        ReasonReact.NoUpdate
    },
    render: ({state: {loading, notifications}, handle, reduce}) =>
        <div style=(Style.container)>
            <div style=(Style.header)>
                (show(t("notifications")))
            </div>
            <LinearLoading loading />
            <div
                ref=(handle(setBoxRef))
                style=(Style.notifications)
                onScroll=(reduce(loadMoreNotifications))>
            {
                switch (Array.length(notifications), loading) {
                | (0, false) => <div style=(Style.nothing)>(show(t("nothing")))</div>
                | _ => notifications
                |> Js.Array.map(notification => <div>(show(notification##date))</div>)
                |> ReasonReact.arrayToElement
                }
            }
            </div>
        </div>
}
