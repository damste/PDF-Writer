# Snake Game per PDF-Writer

## Descrizione

Questo è un gioco Snake implementato come test per la libreria PDF-Writer. Il gioco combina l'intrattenimento classico del Snake con la potenza della generazione di PDF.

## Caratteristiche

- **Due modalità di esecuzione**:
  - **Modalità Test**: Simulazione automatica deterministica per CI/testing
  - **Modalità Interattiva**: Gioco Snake completo controllabile dall'utente
- **Controlli intuitivi**: Usa W/A/S/D per muoverti, Q per uscire (modalità interattiva)
- **Generazione PDF automatica**: Al termine viene generato un report PDF con le statistiche
- **Multipiattaforma**: Funziona su Windows, Linux e macOS
- **Compatibile con CI**: Non blocca l'esecuzione automatica dei test

## Come Usare

### Modalità Test (Default)
1. Compila il progetto PDF-Writer con il nuovo test incluso
2. Esegui il test playground: `./PDFWriterTestPlayground`
3. Seleziona il test "SnakeGameTest" dalla categoria "Games"
4. Il test eseguirà automaticamente una simulazione deterministica e genererà il PDF

### Modalità Interattiva
1. Imposta la variabile d'ambiente: `export SNAKE_INTERACTIVE=1`
2. Esegui il test playground: `./PDFWriterTestPlayground`
3. Seleziona il test "SnakeGameTest" dalla categoria "Games"
4. Usa i controlli:
   - **W**: Muovi su
   - **A**: Muovi sinistra  
   - **S**: Muovi giù
   - **D**: Muovi destra
   - **Q**: Esci dal gioco

## Obiettivo

- Raccogli il cibo (simbolo `*`) per far crescere il serpente
- Evita di colpire i muri o te stesso
- Cerca di ottenere il punteggio più alto possibile!

## Report PDF

Al termine di ogni partita, il gioco genera automaticamente un file PDF chiamato `SnakeGameReport.pdf` che contiene:

- Punteggio finale
- Lunghezza finale del serpente
- Durata della partita
- Valutazione delle performance
- Rappresentazione grafica semplice del serpente

## Valutazione Performance

- **100+ punti**: ECCELLENTE! Sei un maestro del Snake!
- **50-99 punti**: BUONO! Continua così!
- **20-49 punti**: DISCRETO. Puoi migliorare!
- **0-19 punti**: PRINCIPIANTE. Continua a praticare!

## Implementazione Tecnica

Il gioco è implementato in C++ e utilizza:
- **Modalità Test**: Simulazione deterministica con AI semplice (seed fisso per riproducibilità)
- **Modalità Interattiva**: Logica di gioco in tempo reale con input non bloccante
- Rendering console cross-platform (solo in modalità interattiva)
- Integrazione completa con la libreria PDF-Writer per la generazione di report
- Gestione del tempo per statistiche accurate
- Gestione robusta dei font (fallback se i font non sono disponibili)

## File Coinvolti

- `PDFWriterTestPlayground/SnakeGameTest.h` - Header con definizioni delle classi
- `PDFWriterTestPlayground/SnakeGameTest.cpp` - Implementazione del gioco e generazione PDF
- Aggiornamenti al `CMakeLists.txt` per includere i nuovi file nel build

Buon divertimento! 🐍
