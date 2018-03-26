exception Graphql_error(string);

let sendQuery = (q) => {
    let open Js.Promise;
    let open Fetch;
    let body = [("query", Js.Json.string(q##query)), ("variables", q##variables)]
    |> Js.Dict.fromList
    |> Js.Json.object_
    |> Js.Json.stringify
    |> BodyInit.make;
    fetchWithInit(
        "http://app.rung.plutao/api/graphql",
        RequestInit.make(
            ~credentials=Include,
            ~method_=Post,
            ~headers=HeadersInit.make({"content-type": "application/json"}),
            ~body, ()))
    |> then_((response) =>
        switch (Response.ok(response)) {
        | true =>
            Response.json(response)
            |> then_((data) =>
                switch (Js.Json.decodeObject(data)) {
                | Some(obj) => resolve(Js.Dict.unsafeGet(obj, "data"))
                | None => reject(Graphql_error("Response is not an object"))
                })
        | false =>
            reject(Graphql_error("Request failed: " ++ Response.statusText(response)))
        })
};

let request = (~method_=Fetch.Get, path) => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api" ++ path,
        Fetch.RequestInit.make(~credentials=Include, ~method_, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));

let login = (email, password) => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api/login/",
        Fetch.RequestInit.make(
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.make({"content-type": "application/json"}),
            ~body=Fetch.BodyInit.make({j|
                { "email": "$(email)", "password": "$(password)" }
            |j}),
            ~method_=Post, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));
