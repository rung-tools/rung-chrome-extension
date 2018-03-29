exception Graphql_error(string);

let endpoint = "http://app.rung.plutao/api";
let client = "http://app.rung.plutao/";

let sendQuery = (q) => {
    let open Js.Promise;
    let open Fetch;
    let body = [("query", Js.Json.string(q##query)), ("variables", q##variables)]
    |> Js.Dict.fromList
    |> Js.Json.object_
    |> Js.Json.stringify
    |> BodyInit.make;
    fetchWithInit(
        endpoint ++ "/graphql",
        RequestInit.make(
            ~credentials=Include,
            ~method_=Post,
            ~headers=HeadersInit.make({"content-type": "application/json"}),
            ~body, ()))
    |> then_((response) => Response.ok(response)
        ? Response.json(response)
        : reject(Graphql_error("Request failed: " ++ Response.statusText(response))))
    |> then_((data) =>
        switch (Js.Json.decodeObject(data)) {
        | Some(obj) => Js.Dict.unsafeGet(obj, "data") |> q##parse |> resolve
        | None => reject(Graphql_error("Response is not an object"))
        })
};

let request = (~method_=Fetch.Get, path) => Js.Promise.(
    Fetch.fetchWithInit(
        endpoint ++ path,
        Fetch.RequestInit.make(~credentials=Include, ~method_, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));

let login = (email, password) => Js.Promise.(
    Fetch.fetchWithInit(
        endpoint ++ "/login",
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
