
type storageChanges = {.
    "unread": {.
        "oldValue": int,
        "newValue": int
    }
};

type notification = {.
    "readDate": option(string)
};

type storage = {.
    "unread": int
};

type chrome('a) = {.
    "browserAction": {.
        "setBadgeBackgroundColor": [@bs.meth] {."color": string} => unit,
        "setBadgeText": [@bs.meth] {."text": string} => unit
    },
    "alarms": {.
        "create": [@bs.meth] {."periodInMinutes": int} => unit,
        "onAlarm": {.
            "addListener": [@bs.meth] (unit => unit) => unit
        }
    },
    "storage": {.
        "sync": {.
            "set": [@bs.meth] 'a => unit
        },
        "onChanged": {.
            "addListener": [@bs.meth] (storageChanges => unit) => unit
        }
    },
    "i18n": {.
        "getMessage": [@bs.meth] string => string
    }
};

[@bs.val] external chrome : chrome(storage) = "chrome";

[@bs.val] external parseNotifications : string => Js.Array.t(notification) = "JSON.parse";

let updateNotifications = () => Js.Promise.(
    Fetch.fetchWithInit(
        "https://app.rung.com.br/api/notifications",
        Fetch.RequestInit.make(~credentials=Include, ()))
    |> then_((response) => {
        Fetch.Response.ok(response)
            ? Fetch.Response.text(response)
            : Js.Exn.raiseError("")
    })
    |> then_((text) =>
        parseNotifications(text)
        |> Js.Array.filter((notification) =>
            switch (notification##readDate) {
            | Some(_date) => false
            | None => true
            })
        |> (notifications) => {"unread": Array.length(notifications)}
        |> (content) => chrome##storage##sync##set(content)
        |> resolve
    )
    |> catch((_error) => chrome##browserAction##setBadgeText({"text": "..."}) |> resolve))
    |> ignore;

[@bs.new] external notify : (string, {."icon": string, "body": string}) => unit = "Notification";

let observeNotifications = (changes) => {
    let newValue = changes##unread##newValue;
    let oldValue = changes##unread##oldValue;

    switch newValue {
    | 0 => ()
    | n => {
        let text = n > 99 ? "99+" : string_of_int(n);
        chrome##browserAction##setBadgeText({"text": text});
        if (newValue != oldValue) {
            chrome##i18n##getMessage("unreadNotifications")
            |> Js.String.replace("{{AMOUNT}}", text)
            |> (body) => notify("", {"icon": "/public/resources/rung.png", "body": body})
        }
    }}
};

chrome##browserAction##setBadgeBackgroundColor({"color": "#FFA000"});
chrome##alarms##create({"periodInMinutes": 30});
chrome##storage##sync##set({"unread": 0});

chrome##alarms##onAlarm##addListener(updateNotifications);
chrome##storage##onChanged##addListener(observeNotifications);

updateNotifications();