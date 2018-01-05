[@bs.module "rmwc/LinearProgress"]
external linearProgress : ReasonReact.reactClass = "default";

let make = (~determinate, children) =>
    ReasonReact.wrapJsForReason(
        ~reactClass=linearProgress,
        ~props={
            "determinate": Js.Boolean.to_js_boolean(determinate)
        },
        children
    );
