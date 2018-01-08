type storageChanges = {.
    "unread": {.
        "oldValue": int,
        "newValue": int
    },
    "user": {.
        "newValue": string
    }
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
    },
    "tabs": {.
        "create": [@bs.meth] {."url": string} => unit
    }
};

type storage = {.
    "unread": int,
    "user": string
};

[@bs.val]
external chrome : chrome(storage) = "chrome";
