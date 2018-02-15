let component = ReasonReact.statelessComponent("Notification");

[@bs.new]
external now : unit => Js.Date.t = "Date";

module DateFns = {
    [@bs.module "date-fns"]
    external distanceInWordsToNow : Js.Date.t => string = "";
};

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;

let make = (~text, ~color, ~icon, ~onClick, ~read, _children) => {
    ...component,
    render: (self) =>
        <li className=("collection-item avatar notification " ++ (read ? "" : "unread")) onClick>
            <i className=("material-icons circle " ++ color)>(ReasonReact.stringToElement(icon))</i>
            <h6>(ReasonReact.stringToElement(text))</h6>
            <p>(ReasonReact.stringToElement(DateFns.distanceInWordsToNow(now())))</p>
        </li>
}

