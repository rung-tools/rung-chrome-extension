type action =
    | ChangeEmail(string)
    | ChangePassword(string)
    | SetLoginError;

type state = {
    email: string,
    password: string,
    error: bool,
    loading: bool,
    passwordField: ref(option(Dom.element))
};

let component = ReasonReact.reducerComponent("Login");

let reducer = (action, state) =>
    switch action {
    | ChangeEmail(email) => ReasonReact.Update({...state, email})
    | ChangePassword(password) => ReasonReact.Update({...state, password})
    | SetLoginError => ReasonReact.Update({...state, loading: false, error: true})
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
    let loading = ReactDOMRe.Style.make(
        ~position="absolute",
        ~top="0px",
        ~right="0px",
        ~left="0px", ());
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

let handleSubmit = (_event) => SetLoginError;

let handleEmailKeyDown = (event, {ReasonReact.state}) =>
    switch (ReactEventRe.Keyboard.keyCode(event), state.passwordField^, String.trim(state.email) != "") {
    | (13, Some(password), true) => ReactDOMRe.domElementToObj(password)##focus()
    | _ => ()
    };

let handlePasswordKeyDown = (event, {ReasonReact.state, ReasonReact.reduce}) =>
    switch (ReactEventRe.Keyboard.keyCode(event), String.trim(state.password) != "") {
    | (13, true) => reduce(handleSubmit, ())
    | _ => ()
    };

let handleSignUp = (_event, _self) => Chrome.(
    chrome##tabs##create({"url": "https://app.rung.com.br/signup/"}));

let setPasswordRef = (theRef, {ReasonReact.state}) =>
    state.passwordField := Js.Nullable.to_opt(theRef);

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    render: ({state: {email, password, loading, error}, reduce, handle}) =>
        <div style=(Style.container)>
            <div style=(Style.loading)>
            {
                switch loading {
                | true =>
                    <div className="progress">
                        <div className="indeterminate" />
                    </div>
                | false => ReasonReact.nullElement
                }
            }
            </div>
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
                        onClick=(reduce(handleSubmit))>
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
