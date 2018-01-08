let component = ReasonReact.statelessComponent("Notification");

let make = (~text, ~color, ~icon, _children) => {
    ...component,
    render: (_self) =>
        <li className="collection-item avatar">
            <i className=("material-icons circle " ++ color)>(ReasonReact.stringToElement(icon))</i>
            <h6>(ReasonReact.stringToElement(text))</h6>
            <p>(ReasonReact.stringToElement("..."))</p>
        </li>
}

