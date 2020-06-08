/**
 * create my selector
 *
 * use selector to select next bitrate
 * next_quality = selector->select(…)
 * use selector to report error
 * selector->reportError(…)
 *
 *  HighestQualitySelector:
 *  •  Always selects highest bitrate
 *  •  Processes HTTP errors, implements retry logic
 *  AdaptiveQualitySelector:
 *  •  Selects best quality given the current buffer level and network speed
 *  •  Processes HTTP errors, implements retry logic
 *  LowestQualitySelector:
 *  •  Always selects lowest quality
 *  •  Processes HTTP errors, implements retry logic
 */

class QualitySelector {
public:

  enum QualityType {
    Highest,
    Adaptive,
    Lowest,
  };

  std::shared_ptr<QualitySelector> CreateQualitySelector(QualityType type, const std::vector<int64_t>& available_bitrates);
  
  virtual int64_t getNextBitrate() = 0;

  void processHTTPError(int errorCode);

private:
  QualitySelector(const std::vector<int64_t>& available_bitrates) : available_bitrates_(available_bitrates_) {}
  
protected:
  struct Ecapsulations;

  std::unique_ptr<Ecapsulations> abstration;
}

class HighestQualitySelector : public QualitySelector {
public:
  int64_t getNextBitrate() override;
};

class AdaptiveQualitySelector : public QualitySelector {
public:
  int64_t getNextBitrate() override;
};

class LowestQualitySelector : public QualitySelector {
public:
  int64_t getNextBitrate() override;
};
