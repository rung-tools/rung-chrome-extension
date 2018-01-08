type action =
    | Ignore;

type state = {
    page: int,
    notifications: Js.Array.t(string),
    loading: bool,
    boxRef: ref(option(Dom.element))
};

let component = ReasonReact.reducerComponent("NotificationList");

let initialState = () => {
    page: 0,
    notifications: [||],
    loading: true,
    boxRef: ref(None)
};

let reducer = (action, state) => ReasonReact.NoUpdate;

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

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    didMount: (_self) => ReasonReact.NoUpdate,
    render: ({state: {loading, notifications}, reduce, handle}) =>
        <div style=(Style.container) onClick=(reduce((_) => Ignore))>
            <div style=(Style.header)>
                (show(t("notifications")))
            </div>
            <LinearLoading loading />
            <div
                ref=(handle(setBoxRef))
                style=(Style.notifications)>
            {
                switch (Array.length(notifications), loading) {
                | (0, true) => <div style=(Style.nothing)>(show(t("nothing")))</div>
                | n => ReasonReact.nullElement
                }
            }
            </div>
        </div>
}
