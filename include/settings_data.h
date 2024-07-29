struct SettingsData {
  int length = 5;
  int width = 5;
  int numDots = 3;
  int numInitialPowerups = 5;
  int powerupPlacementFrequency = 3;
  int numInitialCrumblies = 3;
  int barrierDensity = 4;
  int numDeletes = 3;
  int numCreates = 3;

  /**
   * @brief Construct a new Settings Data object
   */
  SettingsData() = default;

  void edit();
  void tabulate() const;
};