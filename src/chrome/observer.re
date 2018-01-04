open Fetch;

type chrome('a) = {.
    "browserAction": {.
        "setBadgeBackgroundColor": [@bs.meth] {."color": string} => unit,
        "setBadgeText": [@bs.meth] {."text": string} => unit
    },
    "alarms": {.
        "create": [@bs.meth] {."periodInMinutes": int} => unit
    },
    "storage": {.
        "sync": {.
            "set": [@bs.meth] 'a => unit
        }
    }
};

type storage = {.
    "unread": int
};

[@bs.val] external chrome : chrome(storage) = "chrome";

let updateNotifications = () => Js.Promise.(
    fetchWithInit("https://app.rung.com.br/api/notifications", RequestInit.make(~credentials=Include, ()))
    |> then_((response) => {
        Response.ok(response)
            ? Response.json(response)
            : Js.Exn.raiseError("")
    })
    |> then_((json) => Js.log(json) |> resolve)
    |> catch((_error) => chrome##browserAction##setBadgeText({"text": ":D"}) |> resolve));


chrome##browserAction##setBadgeBackgroundColor({"color": "#FFA000"});
chrome##alarms##create({"periodInMinutes": 30});
chrome##storage##sync##set({"unread": 0});

chrome##browserAction##setBadgeText({"text": "99"});

updateNotifications();