let request = (path) => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api" ++ path,
        Fetch.RequestInit.make(~credentials=Include, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));

let login = (email, password) => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api/login/",
        Fetch.RequestInit.make(
            ~credentials=Include,
            ~body=Fetch.BodyInit.make({j|
                { "email": "$(email)", "password": "$(password)" }
            |j}),
            ~method_=Post, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));