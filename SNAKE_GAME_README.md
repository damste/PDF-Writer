# Snake Game per PDF-Writer

## Descrizione

Questo è un gioco Snake implementato come test per la libreria PDF-Writer. Il gioco combina l'intrattenimento classico del Snake con la potenza della generazione di PDF.

## Caratteristiche

- **Gioco Snake completo**: Controlla il serpente, raccogli il cibo, evita le collisioni
- **Controlli intuitivi**: Usa W/A/S/D per muoverti, Q per uscire
- **Generazione PDF automatica**: Al termine del gioco viene generato un report PDF con le statistiche
- **Multipiattaforma**: Funziona su Windows, Linux e macOS

## Come Giocare

1. Compila il progetto PDF-Writer con il nuovo test incluso
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
- Logica di gioco in tempo reale con input non bloccante
- Rendering console cross-platform
- Integrazione completa con la libreria PDF-Writer per la generazione di report
- Gestione del tempo per statistiche accurate

## File Coinvolti

- `PDFWriterTestPlayground/SnakeGameTest.h` - Header con definizioni delle classi
- `PDFWriterTestPlayground/SnakeGameTest.cpp` - Implementazione del gioco e generazione PDF
- Aggiornamenti al `CMakeLists.txt` per includere i nuovi file nel build

Buon divertimento! 🐍
