import {Action} from './actions'
import {definePlan, isValidPlan, next} from './planner'
import {State} from './state'

export class Npc {
  private _state: State // estado interno atual do NPC
  private _goal: State // estado objetivo do NPC
  private _actions: Action[]
  private _plan: Action[]

  constructor(state: State = {}, actions: Action[]) {
    this._state = {...state}
    this._goal = {}
    this._actions = actions
    this._plan = []
  }

  get state() {
    return this._state
  }
  get plan() {
    return this._plan
  }

  get goal() {
    return this._goal
  }

  set goal(goal: State) {
    this._goal = {...goal}
    this._plan = definePlan(this._state, this._goal, this._actions)
  }

  execute() {
    if (isValidPlan(this._state, this._goal, this._plan)) {
      const action = this._plan.shift() as Action
      console.log(`run action: ${action.name}`)
      this._state = next(this.state, action)
    }
  }
}
