let component = ReasonReact.statelessComponent("Notification");

[@bs.new]
external now : unit => Js.Date.t = "Date";

module DateFns = {
    [@bs.module "date-fns"]
    external distanceInWordsToNow : Js.Date.t => string = "";
};

let t = key => Chrome.(chrome##i18n##getMessage(key));
let show = ReasonReact.stringToElement;

let handleClick = (task) =>
    switch (Js.Nullable.to_opt(task)) {
    | Some(id) => Chrome.(chrome##tabs##create({"url": "https://app.rung.com.br/i/" ++ id}))
    | None => ()
    };

let make = (~text, ~color, ~icon, ~task, _children) => {
    ...component,
    render: (self) =>
        <li className="collection-item avatar notification" onClick=(self.handle((_event, _self) => handleClick(task)))>
            <i className=("material-icons circle " ++ color)>(ReasonReact.stringToElement(icon))</i>
            <h6>(ReasonReact.stringToElement(text))</h6>
            <p>(ReasonReact.stringToElement(DateFns.distanceInWordsToNow(now())))</p>
        </li>
}

