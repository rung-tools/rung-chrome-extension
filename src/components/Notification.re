let component = ReasonReact.statelessComponent("Notification");

[@bs.new]
external now : string => Js.Date.t = "Date";

module DateFns = {
    [@bs.module "date-fns"]
    external distanceInWordsToNow : Js.Date.t => string = "";
};

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;

let make = (~text, ~color, ~icon, ~onClick, ~read, ~date, _children) => {
    ...component,
    render: (_self) =>
        <li className=("collection-item avatar notification " ++ (read ? "" : "unread")) onClick>
            <i className=("material-icons circle " ++ color)>(ReasonReact.stringToElement(icon))</i>
            <h6>(ReasonReact.stringToElement(text))</h6>
            <p>(ReasonReact.stringToElement(DateFns.distanceInWordsToNow(now(date))))</p>
        </li>
}

