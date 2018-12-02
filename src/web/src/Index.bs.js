'use strict';

var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Repl$ReactTemplate = require("./Repl.bs.js");

ReactDOMRe.renderToElementWithId(ReasonReact.element(undefined, undefined, Repl$ReactTemplate.make(/* array */[])), "repl");

/*  Not a pure module */
