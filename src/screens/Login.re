type action =
    | ChangeEmail(string)
    | ChangePassword(string)
    | PressEnterOnEmail
    | Ignore;

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
    | PressEnterOnEmail => ReasonReact.SideEffects((_self) => {
        switch (String.trim(state.email) != "", state.passwordField^) {
        | (true, Some(password)) => ReactDOMRe.domElementToObj(password)
        |> (password) => password##focus()
        | _ => ()
        }
    })
    | Ignore => ReasonReact.NoUpdate
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

let handleEmailKeyDown = (event) =>
    switch (ReactEventRe.Keyboard.keyCode(event)) {
    | 13 => PressEnterOnEmail
    | _ => Ignore
    };

let setPasswordRef = (theRef, {ReasonReact.state}) =>
    state.passwordField := Js.Nullable.to_opt(theRef);

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;
let make = (_children) => {
    ...component,
    initialState,
    reducer,
    render: ({state: {email, password, loading}, reduce, handle}) =>
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
                        onKeyDown=(reduce(handleEmailKeyDown))
                        value=email
                    />
                    <input
                        ref=(handle(setPasswordRef))
                        placeholder=(t("password"))
                        _type="password"
                        style=(Style.input)
                        onChange=(reduce(handleChangePassword))
                        value=password
                    />
                </div>
                <div style=(Style.bottom)>
                    <a className="waves-effect waves-light btn">
                        (show(t("login")))
                    </a>
                    <div style=(Style.register)>
                        (show(t("noAccount")))
                        <a style=(Style.signup)>
                            (show(t("signup")))
                        </a>
                    </div>
                </div>
            </div>
        </div>
}
