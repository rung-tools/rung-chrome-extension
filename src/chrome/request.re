let request = (path) => Js.Promise.(
    Fetch.fetchWithInit(
        "http://app.rung.plutao/api" ++ path,
        Fetch.RequestInit.make(~credentials=Include, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));

let login = (email, password) => Js.Promise.(
    Fetch.fetchWithInit(
        "http://app.rung.plutao/api/login/",
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
