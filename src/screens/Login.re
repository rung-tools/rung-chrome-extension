type action =
    | ChangeEmail
    | ChangePassword;

type state = {
    email: string,
    password: string,
    error: bool,
    loading: bool,
};

let component = ReasonReact.reducerComponent("Login");

let reducer = (action, _state) =>
    switch action {
    | ChangeEmail => ReasonReact.NoUpdate
    | ChangePassword => ReasonReact.NoUpdate
    };

let initialState = () => {email: "", password: "", error: false, loading: false};

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
        ~paddingTop="10px", ());
    let bottom = ReactDOMRe.Style.make(
        ~textAlign="center",
        ~padding="12px", ());
    let signup = ReactDOMRe.Style.make(
        ~marginLeft="6px",
        ~cursor="pointer", ());
    let error = ReactDOMRe.Style.make(
        ~color="#F44336", ())
};

let make = (_children) => {
    ...component,
    initialState,
    reducer,
    render: ({state: {email, loading}}) =>
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
                        placeholder=(Chrome.(chrome##i18n##getMessage("email")))
                        _type="text"
                        style=(Style.input)
                        value=email
                    />
                </div>
            </div>
        </div>
}
