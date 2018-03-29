let component = ReasonReact.statelessComponent("Notification");

[@bs.new]
external now : string => Js.Date.t = "Date";

module DateFns = {
    [@bs.module "date-fns"]
    external distanceInWordsToNow : Js.Date.t => string = "";
};

let t = key => Chrome.(chrome##i18n##getMessage(key));

let format = (fields, text) => {
    let stringify = (transformer, value) =>
        switch value {
        | Some(content) => transformer(content)
        | None => ""
        };
    let sidentity = stringify((x) => x);
    let sint = stringify(string_of_int);
    let slist = stringify(Js.Array.joinWith(", "));

    let dictionary = Js.Dict.empty();
    Js.Dict.set(dictionary, "{{app}}", sidentity(fields##app));
    Js.Dict.set(dictionary, "{{card}}", sidentity(fields##card));
    Js.Dict.set(dictionary, "{{comment}}", sidentity(fields##comment));
    Js.Dict.set(dictionary, "{{count}}", sint(fields##count));
    Js.Dict.set(dictionary, "[[followers]]", slist(fields##followers));
    Js.Dict.set(dictionary, "{{responsible}}", sidentity(fields##responsible));

    dictionary
    |> Js.Dict.keys
    |> Js.Array.reduce((string, key) =>
        Js.String.replace(key, Js.Dict.unsafeGet(dictionary, key), string), text)
};

let styles = (type_) =>
    switch type_ {
    | "alerts-created"         => ("list", "green")
    | "alerts-updated"         => ("system_update_alt", "teal")
    | "alerts-deleted"         => ("delete", "red")
    | "alert-comment"          => ("comment", "indigo")
    | "alert-follow"           => ("people", "blue")
    | "alert-unfollow"         => ("do_not_disturb", "red")
    | "task-created"           => ("warning", "orange")
    | "permissions-updated"    => ("security", "pink")
    | "alert-manually-deleted" => ("delete_forever", "red")
    | _                        => ("alarm", "red")
    };

let click = (onClick, url, event, _self) => {
    onClick(event);
    switch url {
    | Some(path) => Chrome.chrome##tabs##create({"url": Request.client ++ path })
    | None => ()
    }
};

let make = (~type_, ~text, ~onClick, ~read, ~date, ~fields, ~url=?, _children) => {
    ...component,
    render: ({handle}) => {
        let (icon, color) = styles(type_);
        let humanText = text
        |> format(fields)
        |> ReasonReact.stringToElement;
        <li
            className=("collection-item avatar notification " ++ (read ? "" : "unread"))
            onClick=handle(click(onClick, url))>
            <i className=("material-icons circle " ++ color)>(ReasonReact.stringToElement(icon))</i>
            <h6>(humanText)</h6>
            <p>(ReasonReact.stringToElement(DateFns.distanceInWordsToNow(now(date))))</p>
        </li>
    }
}
