open Request;

type action =
    | ChangeEmail(string)
    | ChangePassword(string)
    | StartLoading
    | SetLoginError
    | SetLoginSuccess;

type state = {
    email: string,
    password: string,
    error: bool,
    loading: bool,
    passwordField: ref(option(Dom.element))
};

type retainedProps = {
    onSuccess: unit => unit
};

let component = ReasonReact.reducerComponentWithRetainedProps("Login");

let reducer = (action, state) =>
    switch action {
    | ChangeEmail(email) => ReasonReact.Update({...state, email})
    | ChangePassword(password) => ReasonReact.Update({...state, password})
    | StartLoading => ReasonReact.Update({...state, loading: true})
    | SetLoginError => ReasonReact.Update({...state, loading: false, error: true, password: ""})
    | SetLoginSuccess => ReasonReact.Update({...state, loading: false, error: false})
    };

let initialState = () => {
    email: "",
    password: "",
    error: false,
    loading: false,
    passwordField: ref(None)
};

module Style = {
    let container = ReactDOMRe.Style.make(
        ~padding="12px",
        ~textAlign="center", ());
    let content = ReactDOMRe.Style.make(
        ~margin="24px", ());
    let input = ReactDOMRe.Style.make(
        ~width="100%", ());
    let logo = ReactDOMRe.Style.make(
        ~height="32px",
        ~margin="24px 0 0", ());
    let register = ReactDOMRe.Style.make(
        ~width="100%",
        ~textAlign="center",
        ~paddingTop="30px", ());
    let bottom = ReactDOMRe.Style.make(
        ~textAlign="center",
        ~padding="12px", ());
    let signup = ReactDOMRe.Style.make(
        ~marginLeft="6px",
        ~cursor="pointer", ());
    let error = ReactDOMRe.Style.make(
        ~color="#F44336", ())
};

let handleChangeEmail = (event) => event
|> ReactEventRe.Form.target
|> ReactDOMRe.domElementToObj
|> (obj) => ChangeEmail(obj##value);

let handleChangePassword = (event) => event
|> ReactEventRe.Form.target
|> ReactDOMRe.domElementToObj
|> (obj) => ChangePassword(obj##value);

let handleSubmit = (_event, {ReasonReact.state, ReasonReact.reduce, ReasonReact.retainedProps}) => {
    reduce((_) => StartLoading, ());
    Js.Promise.(
        login(state.email, state.password)
        |> then_((_result) => {
            reduce((_) => SetLoginSuccess, ());
            Intercom.boot({
                "app_id": Intercom.appId,
                "email": state.email
            });
            Intercom.track("authenticated inside chrome extension");
            retainedProps.onSuccess()
            |> resolve
        })
        |> catch((_err) => reduce((_) => SetLoginError, ()) |> resolve))
    |> ignore
};

let handleEmailKeyDown = (event, {ReasonReact.state}) =>
    switch (ReactEventRe.Keyboard.keyCode(event), state.passwordField^, String.trim(state.email) != "") {
    | (13, Some(password), true) => ReactDOMRe.domElementToObj(password)##focus()
    | _ => ()
    };

let handlePasswordKeyDown = (event, {ReasonReact.state, ReasonReact.handle}) =>
    switch (ReactEventRe.Keyboard.keyCode(event), String.trim(state.password) != "") {
    | (13, true) => handle(handleSubmit, ())
    | _ => ()
    };

let handleSignUp = (_event, _self) => Chrome.(
    chrome##tabs##create({"url": "https://app.rung.com.br/signup/"}));

let setPasswordRef = (theRef, {ReasonReact.state}) =>
    state.passwordField := Js.Nullable.to_opt(theRef);

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;
let make = (~onSuccess, _children) => {
    ...component,
    retainedProps: {onSuccess: onSuccess},
    initialState,
    reducer,
    render: ({state: {email, password, loading, error}, reduce, handle}) =>
        <div style=(Style.container)>
            <LinearLoading loading />
            <img
                src="./resources/rung-full.png"
                style=(Style.logo)
                draggable=(Js.false_)
            />
            <div style=(Style.content)>
                <div className="input-field">
                    <input
                        autoFocus=(Js.true_)
                        placeholder=(t("email"))
                        _type="text"
                        style=(Style.input)
                        onChange=(reduce(handleChangeEmail))
                        onKeyDown=(handle(handleEmailKeyDown))
                        value=email
                    />
                    <input
                        ref=(handle(setPasswordRef))
                        placeholder=(t("password"))
                        _type="password"
                        style=(Style.input)
                        onChange=(reduce(handleChangePassword))
                        onKeyDown=(handle(handlePasswordKeyDown))
                        value=password
                    />
                </div>
                {
                    switch error {
                    | true =>
                        <div style=(Style.error)>
                            (show(t("loginError")))
                        </div>
                    | false => ReasonReact.nullElement
                    }
                }
                <div style=(Style.bottom)>
                    <a
                        className=("waves-effect waves-light btn" ++ (loading ? " disabled" : ""))
                        onClick=(handle(handleSubmit))>
                        (show(t("login")))
                    </a>
                    <div style=(Style.register)>
                        (show(t("noAccount")))
                        <a style=(Style.signup) onClick=(handle(handleSignUp))>
                            (show(t("signup")))
                        </a>
                    </div>
                </div>
            </div>
        </div>
}
