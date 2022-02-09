import {State} from './state'

export interface Action {
  name: string
  precond: State
  effects: State
  cost: number
}

const evade: Action = {
  name: 'evade',
  precond: {
    beingAttacked: true,
    isWeak: true,
  },
  effects: {
    beingAttacked: false,
  },
  cost: 1,
}

const findAid: Action = {
  name: 'find aid',
  precond: {
    beingAttacked: false,
    isWeak: true,
    closeToAid: false,
  },
  effects: {
    closeToAid: true,
  },
  cost: 1,
}

const collectAid: Action = {
  name: 'collect aid',
  precond: {
    closeToAid: true,
    hasAid: false,
  },
  effects: {
    closeToAid: false,
    hasAid: true,
  },
  cost: 1,
}

const useAid: Action = {
  name: 'use aid',
  precond: {
    hasAid: true,
    isWeak: true,
    beingAttacked: false,
  },
  effects: {
    hasAid: false,
    isWeak: false,
  },
  cost: 1,
}

const wander: Action = {
  name: 'wander',
  precond: {
    hasTarget: false,
    isWeak: false,
  },
  effects: {
    hasTarget: true,
  },
  cost: 1,
}

const pursuit: Action = {
  name: 'pursuit',
  precond: {
    hasTarget: true,
    isWeak: false,
    closeToEnemy: false,
  },
  effects: {
    closeToEnemy: true,
  },
  cost: 1,
}

const kick: Action = {
  name: 'kick',
  precond: {
    hasTarget: true,
    closeToEnemy: true,
    isWeak: false,
    enemyKilled: false,
  },
  effects: {
    enemyKilled: true,
  },
  cost: 1,
}

const fire: Action = {
  name: 'fire',
  precond: {
    hasTarget: true,
    hasGun: true,
    closeToEnemy: false,
    enemyKilled: false,
  },
  effects: {
    enemyKilled: true,
  },
  cost: 1,
}

const findGun: Action = {
  name: 'find gun',
  precond: {
    isWeak: false,
    closeToGun: false,
  },
  effects: {
    closeToGun: true,
  },
  cost: 1,
}

const collectGun: Action = {
  name: 'collect gun',
  precond: {
    hasGun: false,
    closeToGun: true,
  },
  effects: {
    hasGun: true,
    closeToGun: false,
  },
  cost: 1,
}

export const actions = [
  evade,
  findAid,
  collectAid,
  useAid,
  wander,
  pursuit,
  kick,
  findGun,
  collectGun,
  fire,
]
