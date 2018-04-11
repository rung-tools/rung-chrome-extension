[@bs.val]
external appId : string = "APP_ID";

[@bs.val]
external intercom : string => 'a => unit = "Intercom";

let boot = (data) => intercom("boot", data);

let track = (event) => intercom("trackEvent", event)
