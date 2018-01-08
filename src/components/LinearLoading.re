let component = ReasonReact.statelessComponent("LinearLoading");

let style = ReactDOMRe.Style.make(
    ~position="absolute",
    ~top="0px",
    ~right="0px",
    ~left="0px", ());

let make = (~loading, _children) => {
    ...component,
    render: (_self) =>
        <div style>
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
}
