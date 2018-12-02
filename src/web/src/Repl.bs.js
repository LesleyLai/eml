'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");

function record_to_elem(record) {
  return React.createElement("div", undefined, React.createElement("p", undefined, "> " + record[/* command */0]), $$Array.map((function (line) {
                    return React.createElement("p", undefined, "  " + line);
                  }), record[/* response */1].split("\n")));
}

var component = ReasonReact.reducerComponent("Example");

function make(_children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              return React.createElement("div", undefined, $$Array.of_list(List.rev(List.map(record_to_elem, self[/* state */1][/* history */0]))), React.createElement("span", undefined, "> "), React.createElement("input", {
                              style: {
                                background: "#222",
                                border: "none",
                                color: "#FFFFFF",
                                width: "90%"
                              },
                              autoFocus: true,
                              value: self[/* state */1][/* inputTest */1],
                              onKeyDown: (function ($$event) {
                                  var match = $$event.which;
                                  if (match !== 13) {
                                    return /* () */0;
                                  } else {
                                    return Curry._1(self[/* send */3], /* Enter */0);
                                  }
                                }),
                              onChange: (function ($$event) {
                                  return Curry._1(self[/* send */3], /* InputText */[$$event.target.value]);
                                })
                            }));
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* history : [] */0,
                      /* inputTest */""
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (action) {
                return /* Update */Block.__(0, [/* record */[
                            /* history */state[/* history */0],
                            /* inputTest */action[0]
                          ]]);
              } else {
                var jsInterpret = ( 
          function(s) {
            return Pointer_stringify(interpret(s));
          }
        );
                var request = state[/* inputTest */1];
                var response = Curry._1(jsInterpret, request);
                return /* Update */Block.__(0, [/* record */[
                            /* history : :: */[
                              /* record */[
                                /* command */request,
                                /* response */response
                              ],
                              state[/* history */0]
                            ],
                            /* inputTest */""
                          ]]);
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.record_to_elem = record_to_elem;
exports.component = component;
exports.make = make;
/* component Not a pure module */
