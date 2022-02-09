import {actions} from './actions'
import {Npc} from './npc'
import {State} from './state'

const state: State = {
  beingAttacked: false,
  closeToAid: false,
  hasAid: false,
  hasTarget: false,
  isWeak: true,
  closeToEnemy: false,
  hasGun: false,
  closeToGun: false,
  enemyKilled: false,
}

const goal: State = {
  enemyKilled: true,
}

const soldier = new Npc(state, actions)
soldier.goal = goal

console.log(soldier.state)

while (soldier.plan.length > 0) {
  soldier.execute()
}

console.log(soldier.state)
