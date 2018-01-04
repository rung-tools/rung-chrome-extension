type router = {.
    "init": [@bs.meth] (string => unit)
};

module Top = {
    type action =
        | GoToNotifications;

    type screen =
        | NotificationsScreen
        | LoginScreen
        | LoadingScreen;

    type state = {
        screen: screen
    };

    let component = ReasonReact.reducerComponent("Top");

    let reducer = (action, _state) =>
        switch action {
        | GoToNotifications => ReasonReact.Update({screen: NotificationsScreen})
        };

    let initialState = () => {screen: LoadingScreen};

    let make = (_children) => {
        ...component,
        initialState,
        reducer,
        render: ({state: {screen}}) =>
            switch screen {
            | NotificationsScreen => <div></div>
            | LoginScreen => <div></div>
            | LoadingScreen => <div></div>
            }
    }
 };

ReactDOMRe.renderToElementWithId(<Top />, "app");
