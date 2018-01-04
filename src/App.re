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
            | NotificationsScreen => <div>(ReasonReact.stringToElement("Not"))</div>
            | LoginScreen => <div>(ReasonReact.stringToElement("Log"))</div>
            | LoadingScreen => <LinearProgressRe determinate=false />
            }
    }
 };

ReactDOMRe.renderToElementWithId(<Top />, "app");
