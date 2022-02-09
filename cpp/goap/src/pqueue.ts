/** Fila de prioridade genérica. */
export class PQueue<T> {
  /** Cria uma fila cuja prioridade é dada por uma função que recebe dois elementos e retorna o de maior prioridade. */
  constructor(private priorFunc: (a: T, b: T) => T, private queue: T[] = []) {}

  /** Insere um elemento na fila de prioridade. */
  push(data: T) {
    this.queue.push(data)
  }

  /** Remove e retorna o elemento de maior prioridade (de acordo com a função passada no construtor) */
  pop(): T | undefined {
    if (this.queue.length > 0) {
      const elm = this.queue.reduce((prev, curr) => this.priorFunc(prev, curr))
      const idx = this.queue.findIndex(e => e === elm)
      this.queue.splice(idx, 1)
      return elm
    }
    return undefined
  }

  /** Verifica se a fila está vazia. */
  isEmpty() {
    return this.queue.length === 0
  }

  /**
   * Encontra um elemento na fila (não remove).
   * Se uma função for passada, a comparação será realizada a partir da função.
   */
  find(data: T | ((e: T) => boolean)) {
    if (typeof data === 'function') {
      const comp = data as (e: T) => boolean
      return this.queue.find(elm => comp(elm))
    }
    return this.queue.find(elm => elm === data)
  }
}
