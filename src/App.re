module Top = {
    type action =
        | GoToNotifications
        | GoToLogin
        | CheckAuthentication;

    type screen =
        | NotificationsScreen
        | LoginScreen
        | LoadingScreen;

    type state = {screen: screen};

    type user = {."name": string};

    [@bs.val]
    external parseUser : string => option(user) = "JSON.parse";

    let component = ReasonReact.reducerComponent("Top");

    let reducer = (action, _state) =>
        switch action {
        | CheckAuthentication => ReasonReact.SideEffects((self) => Js.Promise.(
            Request.request("/whoami")
            |> then_((text) => resolve(Some(parseUser(text))))
            |> catch((_err) => {
                self.reduce((_) => GoToLogin, ());
                resolve(None)
            })
            |> ignore
        ))
        | GoToNotifications => ReasonReact.Update({screen: NotificationsScreen})
        | GoToLogin => ReasonReact.Update({screen: LoginScreen})
        };

    let initialState = () => {screen: LoadingScreen};

    let make = (_children) => {
        ...component,
        initialState,
        reducer,
        didMount: ({reduce}) => {
            reduce((_) => CheckAuthentication, ());
            ReasonReact.NoUpdate
        },
        render: ({state: {screen}}) =>
            switch screen {
            | NotificationsScreen => <div>(ReasonReact.stringToElement("Not"))</div>
            | LoginScreen => <div>(ReasonReact.stringToElement("Log"))</div>
            | LoadingScreen => <LinearProgressRe determinate=false />
            }
    }
 };

ReactDOMRe.renderToElementWithId(<Top />, "app");
