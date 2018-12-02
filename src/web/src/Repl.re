type record = {
  command: string,
  response: string,
}

let record_to_elem(record: record) = {
  <div>
  <p> {ReasonReact.string("> " ++ record.command)} </p>
  {if (String.length(record.response) != 0) {
    <p> {ReasonReact.string("  " ++ record.response)} </p>
  } else {
    ReasonReact.string("")
  } }
  </div>
}

type state = {
  history: list(record),
  inputTest: string
};

type action =
    | Enter
    | InputText(string)

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Example");

/* greeting and children are props. `children` isn't used, therefore ignored.
   We ignore it by prepending it with an underscore */
let make = (_children) => {
  /* spread the other default fields of component here and override a few */
  ...component,

  initialState: () => {history: [], inputTest: ""},

  reducer: (action, state) => {
    switch (action) {
      | InputText(newTest) => {
        ReasonReact.Update({...state, inputTest: newTest})
      }
      | Enter => {
        let jsInterpret: string => string = [%bs.raw {| 
          function(s) {
            return Pointer_stringify(interpret(s));
          }
        |}];
        let request = state.inputTest;
        let response = jsInterpret(request);
        ReasonReact.Update({inputTest: "",
                            history: [{command: request, response: response}, ...state.history]});
      }
      | _ => ReasonReact.NoUpdate
    }
  },

  render: self => {
    <div>
    (
          List.map(record_to_elem, self.state.history)
          -> List.rev
          -> Array.of_list
          -> ReasonReact.array
    )

      <span>
          {ReasonReact.string("> ")}
      </span>
        
      <input
      value=self.state.inputTest
      style=(ReactDOMRe.Style.make(~color="#444444", ~width="90%", ~border="none", ()))
      onKeyDown=(event => switch(ReactEvent.Keyboard.which(event)) {
        | 13 => self.send(Enter)
        | _ => ()
      })
      onChange=(event => {
        self.send(InputText(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value));
      })
      />
    </div>;
    },
};
