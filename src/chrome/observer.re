open Fetch;

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
        }
    }
};

type notification = {.
  "readDate": option(string)
};

[@bs.val] external chrome : chrome({."unread": int}) = "chrome";

[@bs.val] external parseNotifications : string => Js.Array.t(notification) = "JSON.parse";

let updateNotifications = () => Js.Promise.(
    fetchWithInit("https://app.rung.com.br/api/notifications", RequestInit.make(~credentials=Include, ()))
    |> then_((response) => {
        Response.ok(response)
            ? Response.text(response)
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


chrome##browserAction##setBadgeBackgroundColor({"color": "#FFA000"});
chrome##alarms##create({"periodInMinutes": 30});
chrome##storage##sync##set({"unread": 0});

chrome##alarms##onAlarm##addListener(updateNotifications);

updateNotifications();