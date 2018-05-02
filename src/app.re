[%bs.raw {|require('./app.css')|}];

let component =
  ReasonReact.statelessComponent("App");

let make = (_children) => {
  ...component,
  render: _self => {
    <div className="App">
      <header className="App-header">
        <h1 className="App-title">
          (ReasonReact.string("Rock Scissors Paper Game!"))
        </h1>
      </header>
      <Janken />
  </div>
  }
}