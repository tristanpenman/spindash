#pragma once

#include <memory>
#include <vector>

#include <QDialog>
#include <QWidget>

class QCheckBox;
class QComboBox;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPainter;

class Chunk;
class Block;
class Level;
class Palette;
class Pattern;

class ChunkCanvas : public QWidget
{
  Q_OBJECT

public:
  ChunkCanvas(QWidget* parent, std::shared_ptr<Level>& level);

  void setChunkIndex(size_t chunkIndex);
  void setPreviewPalette(size_t paletteIndex);
  void setSelectedBlock(uint16_t blockIndex);
  void setHorizontalFlip(bool enabled);
  void setVerticalFlip(bool enabled);

protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  void drawAt(const QPoint& pos);
  void drawChunk(QPainter& painter, const Chunk& chunk);
  void drawBlock(QPainter& painter, const Block& block, int dx, int dy, bool hFlip, bool vFlip);
  void drawPattern(QPainter& painter, const Pattern& pattern, const Palette& palette, int dx, int dy, bool hFlip, bool vFlip);

  std::shared_ptr<Level> m_level;
  size_t m_chunkIndex;
  size_t m_previewPaletteIndex;
  uint16_t m_selectedBlockIndex;
  bool m_hFlip;
  bool m_vFlip;
  int m_highlightX;
  int m_highlightY;

signals:
  void chunkModified();
};

class ChunkEditor : public QDialog
{
  Q_OBJECT

public:
  ChunkEditor(QWidget* parent, std::shared_ptr<Level>& level);

private:
  QPixmap renderBlockPreview(size_t blockIndex, int scale) const;
  void drawPattern(QImage& image, const Pattern& pattern, const Palette& palette, int dx, int dy, bool hFlip, bool vFlip) const;
  void drawBlockPreview(QImage& image, const Block& block, int dx, int dy) const;
  void populateBlockSelector();
  void updateTitle();

  std::shared_ptr<Level> m_level;

  QComboBox* m_chunkCombo;
  QComboBox* m_paletteCombo;
  QListWidget* m_blockList;
  QCheckBox* m_hFlipCheckBox;
  QCheckBox* m_vFlipCheckBox;
  ChunkCanvas* m_canvas;

  size_t m_chunkIndex;
  size_t m_previewPaletteIndex;
  bool m_dirty;

private slots:
  void blockChanged(QListWidgetItem* current, QListWidgetItem* previous);
  void horizontalFlipChanged(int state);
  void paletteChanged(int paletteIndex);
  void chunkChanged(int chunkIndex);
  void chunkModified();
  void verticalFlipChanged(int state);

signals:
  void chunksModified();
};
