/**
 * Um estado é um objeto com propriedades cujos valores são boleanos ou numéricos.
 */
export interface State {
  [key: string]: number | boolean
}

/**
 * Verifica se os valores de todas propriedades de um estado são iguais aos valores das mesmas
 * propriedades em outro estado. Diz-se que o estado A "casa" com B se, para cada propriedade
 * em A, se ela existe em B, então seu valor é o mesmo em A e B.
 */
export function match(a: State, b: State) {
  return Object.keys(a).every(k => b[k] === undefined || a[k] === b[k])
}

/**
 * Verifica se pelo menos uma propriedade em A possui o mesmo valor em B.
 */
export function partialMatch(a: State, b: State) {
  return Object.keys(b).some(key => b[key] === a[key])
}

/**
 * Verifica se dois estados são iguais. Eles são considerados iguais se possuem o mesmo número
 * de propriedades e todas possuem os mesmos valores.
 */
export function isEqual(a: State, b: State) {
  const k1 = Object.keys(a)
  const k2 = Object.keys(b)
  return k1.length === k2.length && k1.every(k => a[k] === b[k])
}

/**
 * Retorna a distância entre dois estados. A distância é calculada pela quantidade de propriedades
 * que um difere do outro.
 */
export function distance(s1: State, s2: State) {
  const k1 = Object.keys(s1)
  const k2 = Object.keys(s2)
  const equals = k1.filter(k => s1[k] === s2[k]) // props cujos valores são iguais em s1 e s2
  const min = k1.length < k2.length ? k1.length : k2.length // quem tem menos props?
  return min - equals.length // retorna a menor qde de props com exceção das props iguais.
}
