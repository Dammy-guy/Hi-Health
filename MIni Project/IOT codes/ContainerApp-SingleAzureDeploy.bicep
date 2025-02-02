targetScope = 'subscription'

@minLength(6)
@maxLength(16)
@description('Basename that is used to name provisioned resources. Should be alphanumeric, at least 6 characters and less than 16 characters.')
param baseName string

@description('Location where the resources are deployed. For a list of Azure regions where Azure Health Data Services are available, see [Products available by regions](https://azure.microsoft.com/explore/global-infrastructure/products-by-region/?products=health-data-services)')
@allowed([
  'australiaeast'
  'canadacentral'
  'centralindia'
  'eastus'
  'eastus2'
  'francecentral'
  'japaneast'
  'koreacentral'
  'northcentralus'
  'northeurope'
  'qatarcentral'
  'southcentralus'
  'southeastasia'
  'swedencentral'
  'switzerlandnorth'
  'westcentralus'
  'westeurope'
  'westus2'
  'westus3'
  'uksouth'
])
param location string 

@description('Configures how patient, device, and other FHIR resource identities are resolved from the ingested data stream.')
@allowed([
  'Create'
  'Lookup'
  'LookupWithEncounter'
])
param resourceIdentityResolutionType string 

@description('FHIR version that the FHIR Server supports')
@allowed([
  'R4'
])
param fhirVersion string

@description('The URL for the repository where the container code resides.')
param gitRepositoryUrl string = 'https://github.com/microsoft/iomt-fhir.git'

@description('The branch name or commit hash to be used when building the containers, defaults to main.')
param gitBranch string = 'main'

resource resourceGroup 'Microsoft.Resources/resourceGroups@2022-09-01' = {
  name: baseName
  location: location
  tags: {
    IomtFhirConnector: 'ResourceIdentity:${resourceIdentityResolutionType}'
    IomtFhirVersion: fhirVersion
  }
}

module infrastructureSetup 'InfrastructureSetup.bicep' = {
  name: 'infrastructureSetup'
  scope: resourceGroup
  params: {
    baseName: baseName 
    location: location 
    resourceIdentityResolutionType: resourceIdentityResolutionType
    fhirVersion: fhirVersion
  }
}

module uploadTemplates 'UploadTemplates.bicep' = {
  name: 'uploadTemplates'
  scope: resourceGroup
  params: {
    baseName: baseName
    location: location 
    resourceIdentityResolutionType: resourceIdentityResolutionType
    fhirVersion: fhirVersion
  }
  dependsOn: [
    infrastructureSetup
  ]
}

module buildContainerImages 'BuildContainerImages.bicep' = {
  name: 'buildContainerImages'
  scope: resourceGroup
  params: {
    baseName: baseName
    location: location
    resourceIdentityResolutionType: resourceIdentityResolutionType
    fhirVersion: fhirVersion
    gitRepositoryUrl: gitRepositoryUrl
    gitBranch: gitBranch
  }
  dependsOn: [
    infrastructureSetup
    uploadTemplates
  ]
}

module containerAppSetup 'ContainerAppSetup.bicep' = {
  name: 'containerAppSetup'
  scope: resourceGroup
  params: {
    baseName: baseName
    location: location
    resourceIdentityResolutionType: resourceIdentityResolutionType
    fhirVersion: fhirVersion
  }
  dependsOn: [
    infrastructureSetup
    uploadTemplates
    buildContainerImages
  ]
}
