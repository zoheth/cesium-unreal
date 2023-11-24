// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumIonServer.h"
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CesiumEditorSettings.generated.h"

/**
 * Stores Editor settings for the Cesium plugin.
 */
UCLASS(Config = EditorPerProjectUserSettings, meta = (DisplayName = "Cesium"))
class UCesiumEditorSettings : public UDeveloperSettings {
  GENERATED_UCLASS_BODY()

public:
  /**
   * The token representing the signed-in user to Cesium ion. If this is blank
   * or invalid, the Cesium panel will prompt you to log in to Cesium ion with
   * OAuth2. This is set automatically by logging in with the UI; it is not
   * usually necessary to set it directly.
   */
  UPROPERTY(
      Config,
      EditAnywhere,
      Category = "Cesium ion",
      meta = (DisplayName = "User Access Token"))
  FString UserAccessToken;

  /**
   * The Cesium ion server that is currently selected in the user interface.
   */
  UPROPERTY(
      Config,
      EditAnywhere,
      Category = "Cesium ion",
      meta = (DisplayName = "Current Cesium ion Server"))
  TSoftObjectPtr<UCesiumIonServer> CurrentCesiumIonServer;

  UPROPERTY(
      Config,
      EditAnywhere,
      Category = "Cesium ion",
      meta = (DisplayName = "Token Map"))
  TMap<TSoftObjectPtr<UCesiumIonServer>, FString> UserAccessTokenMap;

  void Save();
};
