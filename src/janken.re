[%bs.raw {|require('./janken.css')|}];

module Hand {
  type t
    = Rock
    | Scissors
    | Paper;

  let to_string = hand =>
    switch hand {
    | Rock => {js|グー|js}
    | Scissors => {js|チョキ|js}
    | Paper => {js|パー|js}
    };

  exception UnexpectedNumber(int);

  let random_hand = () =>
    switch (Random.int(3)) {
      | 0 => Rock
      | 1 => Paper
      | 2 => Scissors
      | n => raise(UnexpectedNumber(n))
      };
};

type action
  = Reset
  | Play(Hand.t);

type state
  = Wait
  | Result(Hand.t, Hand.t);

type result =
  | Victory
  | Defeated
  | Draw;

let show_result_message = (player, enemy) => {
  Hand.(
    switch (player, enemy) {
    | (Rock, Scissors) => Victory
    | (Rock, Paper) => Defeated
    | (Scissors, Rock) => Defeated
    | (Scissors, Paper) => Victory
    | (Paper, Rock) => Victory
    | (Paper, Scissors) => Defeated
    | _ => Draw
    }
  )
};

let reducer = (action, _state) =>
  switch action {
  | Reset => ReasonReact.Update(Wait)
  | Play(player) => ReasonReact.Update(Result(player, Hand.random_hand()))
  };

let initialState = () => Wait;

let component = ReasonReact.reducerComponent("Janken");

let t = str => ReasonReact.string(str);

let make = (_children) => {
  ...component,
  reducer,
  initialState,
  render: ({state, send}) => {
    let make_button = hand =>
      <button _type="button" onClick=(_e => send(Play(hand)))>(t(Hand.to_string(hand)))</button>;
    let body =
      switch state {
      | Wait =>
          <div>
            <p>(t({js|じゃんけんゲームをします。|js}))</p>
            <p>(t({js|好きな手を選んでください。|js}))</p>
            (make_button(Hand.Rock))
            (make_button(Hand.Scissors))
            (make_button(Hand.Paper))
          </div>
      | Result(player, enemy) =>
          let message =
            <p>
              (t({js|あなたは|js}))
              <span className="hand">(t(Hand.to_string(player)))</span>
              (t({js|を出しました。|js}))
              (t({js|相手は|js}))
              <span className="hand">(t(Hand.to_string(enemy)))</span>
              (t({js|を出しました。|js}))
            </p>;
          let result =
            switch (show_result_message(player, enemy)) {
            | Victory => 
              <p>
                (t({js|あなたの|js}))
                <span className="result">(t({js|勝ち|js}))</span>
                (t({js|です|js}))
              </p>
            | Defeated =>
              <p>
                (t({js|あなたの|js}))
                <span className="result">(t({js|負け|js}))</span>
                (t({js|です|js}))
              </p>
            | Draw =>
              <p>
                <span className="result">(t({js|引き分け|js}))</span>
                (t({js|です|js}))
              </p>
            };
          <div>
            (message)
            (result)
            <button _type="button" onClick=(_e => send(Reset))>
              (t({js|もう一度遊ぶ|js}))
            </button>
          </div>
      };
    <div id="janken_field">
      (body)
    </div>
  }
}