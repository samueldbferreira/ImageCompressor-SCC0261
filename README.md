# Projeto: Compressão de Imagens

**Aluno:** Samuel de Assunção Ferreira  
**Nº USP:** 12543565  
**Disciplina:** SSC226 - Multimídia  
**Professor:** Rudinei Goularte  
**Ano:** 2025

---

## 📌 Descrição

Este projeto implementa um compressor e descompressor de imagens BMP com suporte a dois modos de compressão:

- **Compressão sem perdas (Lossless)**  
  Utiliza o método das diferenças entre pixels RGB, seguido de codificação de Huffman.

- **Compressão com perdas (Lossy)**  
  Segue o pipeline JPEG simplificado:  
  Conversão para YCbCr → Divisão em blocos 8x8 → Transformada Discreta do Cosseno (DCT) → Quantização → Huffman.

---

## ⚙️ Funcionalidades

- Leitura e escrita de arquivos `.bmp` (24 bits);
- Compressão e descompressão completas nos modos com e sem perdas;
- Construção de árvores de Huffman para codificação de símbolos (diferenciais ou quantizados);
- Escrita e leitura bit a bit de arquivos binários compactados;
- Conversão entre RGB e YCbCr;
- Cálculo da taxa de compressão.

---

## ▶️ Como compilar e executar

```bash
# Compressor
make run-compressor
# Vai escrever a imagem comprimida no arquivo binário no caminho especificado na entrada.

# Descompressor
make run-decompressor
# Irá reconstruir a imagem BMP no arquivo BMP especificado na entrada a partir do aquivo binário também especificado na entrada.
