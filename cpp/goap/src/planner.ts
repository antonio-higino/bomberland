import {Action} from './actions'
import {PQueue} from './pqueue'
import {distance, isEqual, match, partialMatch, State} from './state'

/**
 * Retorna o estado gerado pela execução de uma ação sobre um estado original.
 */
export function next(original: State, action: Action): State {
  return {...original, ...action.effects} // junta e sobreescreve as propriedades
}

/**
 * Retorna o estado que, se uma dada ação for executada, leva ao estado desejado.
 */
function previous(action: Action, target: State): State {
  return {...target, ...action.precond}
}

/**
 * Verifica se as ações de um plano transforma um estado inicial (current) em um estado
 * objetivo (goal). A verificação é feita a partir das precondições de cada ação e os
 * seus efeitos (transformações no estado).
 */
export function isValidPlan(
  initial: State,
  goal: State,
  plan: Action[]
): boolean {
  const result = plan.reduce((state, action) => {
    if (match(action.precond, state)) {
      return next(state, action)
    }
    return {}
  }, initial)
  return match(goal, result)
}

/** Nó do grafo utilizado pelo A*. Encapsula um estado. */
class Node {
  public from?: Edge // aponta para o node de onde foi gerado (nó pai)
  constructor(
    public state: State,
    public gCost: number = Number.MAX_SAFE_INTEGER, // custo real (calculado até o momento)
    public hCost: number = Number.MAX_SAFE_INTEGER // custo estimado até o objetivo (heurística)
  ) {}
}

/** Aresta dirigida do grafo utilizado pelo A*. */
class Edge {
  constructor(
    public action: Action, // ação que transforma um estado em outro.
    public origin: Node, // nó de onde a aresta sai
    public target: Node // e para onde vai.
  ) {}
}

/**
 * Cria uma sequência de ações voltando no caminho do A*, do nó atual até o inicial.
 */
function createPath(node: Node): Action[] {
  const path: Action[] = []
  while (node.from) {
    // quando não houver mais `from` é porque chegou no raiz.
    path.push(node.from.action)
    node = node.from.origin
  }
  return path
}

/**
 * Cria um plano (sequência de ações) para levar um estado inicial a um estado final
 * a partir de um conjunto de ações possíveis.
 * @param initial Estado inicial.
 * @param goal    Estado final
 * @param actions Conjunto de ações possíveis.
 * @returns       Sequência de ações para levar `initial` a `goal`.
 */
export function definePlan(
  initial: State,
  goal: State,
  actions: Action[]
): Action[] {
  const fCost = (node: Node) => node.gCost + node.hCost
  const priorFunc = (a: Node, b: Node) => (fCost(a) < fCost(b) ? a : b)

  const queue = new PQueue(priorFunc) // fila de prioridade
  const closed: Node[] = [] // nós que já saíram da fila (não precisam calcular novamente)

  // 1º no da fila é o estado objetivo (goal). A busca será do no final em direção ao inicial.
  queue.push(new Node(goal, 0, distance(goal, initial)))

  while (!queue.isEmpty()) {
    const currNode = queue.pop() as Node
    if (match(currNode.state, initial)) {
      // se o estado nó atual casar com o inicial, então temos um caminho
      return createPath(currNode)
    }
    closed.push(currNode)
    actions // para cada ação que possui pelo menos um efeito levando ao estado do nó atual
      .filter(action => partialMatch(action.effects, currNode.state))
      .forEach(action => {
        const prevState = previous(action, currNode.state) // calcula o estado anterior à ação
        if (closed.every(n => !isEqual(n.state, prevState))) {
          // se é um estado que não saiu da fila, procura-o na fila
          let prevNode = queue.find(node => isEqual(node.state, prevState))
          if (!prevNode) {
            // se não estiver na fila, cria um novo nó e insere na fila
            prevNode = new Node(prevState)
            queue.push(prevNode)
          }
          const gCost = currNode.gCost + action.cost
          const hCost = distance(prevState, initial)
          if (gCost + hCost < fCost(prevNode)) {
            // se o custo calculado agora for menor que o estimado anteriormente,
            // então cria uma nova transição (aresta) e atualiza os dados
            const edge = new Edge(action, currNode, prevNode)
            prevNode.gCost = gCost
            prevNode.hCost = hCost
            prevNode.from = edge
          }
        }
      })
  }
  // se chegou aqui é porque não alcançou o nó inicial e, portanto, não há plano possível.
  return []
}
