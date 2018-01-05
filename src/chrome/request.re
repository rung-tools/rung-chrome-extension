let request = path => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api" ++ path,
        Fetch.RequestInit.make(~credentials=Include, ()))
    |> then_((response) => Fetch.Response.ok(response)
        ? Fetch.Response.text(response)
        : Js.Exn.raiseError("")));
