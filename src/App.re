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

    type user = {."name": string, "email": string};

    [@bs.val]
    external parseUser : string => user = "JSON.parse";

    let component = ReasonReact.reducerComponent("Top");

    let reducer = (action, _state) =>
        switch action {
        | CheckAuthentication => ReasonReact.SideEffects((self) => Js.Promise.(
            Request.request("/whoami")
            |> then_((text) => {
                let user = parseUser(text);
                self.reduce((_) => GoToNotifications, ());
                Intercom.boot({
                    "app_id": Intercom.appId,
                    "email": user##email,
                    "name": user##name,
                    "created_at": Js.Date.make()
                });
                Intercom.track("opened chrome notification popup");
                resolve(Some(user))
            })
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
        render: ({state: {screen}, reduce}) =>
            switch screen {
            | NotificationsScreen => <NotificationList />
            | LoginScreen => <Login onSuccess=(reduce((_) => GoToNotifications)) />
            | LoadingScreen => <LinearLoading loading=true />
            }
    }
 };

ReactDOMRe.renderToElementWithId(<Top />, "app");
