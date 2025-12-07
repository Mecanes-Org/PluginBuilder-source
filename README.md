# 🇬🇧 PluginBuilder - A Tool for Unreal Engine Developers

![Qt](https://img.shields.io/badge/Qt-6.x-green.svg) ![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg) ![License](https://img.shields.io/badge/License-MIT-yellow.svg)

**PluginBuilder** is a powerful and lightweight desktop tool built with Qt/C++, designed to fully automate the build, packaging, and export process of your Unreal Engine plugins.  
Whether you target a single or multiple engine versions, PluginBuilder streamlines your workflow and produces clean output folders ready for publication on the Unreal Marketplace (Fab).

## 📖 Table of Contents

- [✨ Key Features](#-key-features)
- [🎯 Why PluginBuilder?](#-why-pluginbuilder)
- [🛠️ Built With](#️-built-with)
- [📚 Usage](#-usage)
- [📄 License](#-license)
- [📧 Contact](#-contact)

## ✨ Key Features

- 🔧 **Automatic builds** for plugins targeting multiple Unreal Engine versions.  
- 📦 **Complete packaging pipeline** with structured output folders.  
- 🚀 **One-click export** ready for Fab publication.  
- 📁 **Automatic folder organization** for build outputs.  
- ⚙️ **Fast, clean, and user-friendly interface** developed with Qt.  
- 🧩 Compatible with all plugin types (Blueprint, C++, hybrid).

## 🎯 Why PluginBuilder?

- **Save significant time** with every plugin update.  
- **Avoid build errors** and manual operations.  
- **Fully automate** your publication workflow.  
- **Standardize outputs** for Fab or your clients.

## 🛠️ Built With

This project uses the following technologies:

- ![Qt](https://img.shields.io/badge/Qt-Framework-green)
- ![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
- ![CMake](https://img.shields.io/badge/CMake-Build%20System-orange)

## 📚 Usage

Once the application launches, the main interface appears.  
Below is a detailed guide to each section of the application.

### 1. Loading Window (`LoadingWindow`)

At startup, a loading window appears showing a progress bar while the application initializes and loads the required data.

### 2. Main Window (`MainWindow`)

This is the core interface of PluginBuilder.

![alt text](tuto-images/pluginBuilder/mainWindow.png "MainWindow UI")

- **Unreal Engine Versions List (1)**: Displays all configured engine versions. Check the boxes for the versions you want to build your plugin with.  
- **Plugin Selection (2)**: Notification area showing updates or build completion messages.  
- **Action Buttons (3)**:
  - `🔍︎`: Browse for your plugin’s `.uplugin` file.  
  - `Plugin Name`: Name your plugin.  
  - `Plugin Version`: Set your plugin version.  
  - `Build`: Starts the build and packaging process for selected versions.  
  - `Edit`: Opens the editing window.
    - `Unreal Versions`: Opens the Unreal Engine version management window.  
    - `Settings`: Opens the application settings window.  
  - `About Us`: Shows information about the development team.  
  - `Examples`:
    - ![alt text](tuto-images/examples/FolderExemple.png "Examples Folder")
    - ![alt text](tuto-images/examples/FolderExemple-SIS.png "Examples Folder")

- **Log Area (4)**: Displays build logs, warnings, and status messages in real time — ideal for tracking the whole process.

### 3. Unreal Version Management (`UnrealVersions`)

This window allows you to add, edit, or remove Unreal Engine installations used by PluginBuilder for building.

![alt text](tuto-images/pluginBuilder/unreal-versions.png "UnrealVersions UI")

- **Add a New Version**: Click `Add new Unreal versions` to create a new entry. Provide a name and the path to the Unreal Engine folder (for example, `C:\Program Files\Epic Games\UE_5.4`).  
- **Version List**: Displays all configured engine versions, showing each version’s name (e.g., "UE 5.4") and install path.  
- **Save/Cancel**: Use the buttons at the bottom to confirm or discard changes.  
- `Example`:  
  - ![unreal version example](tuto-images/examples/unreal-version-example.png "Unreal Version Example")

### 4. Settings Window (`Settings`)

This section allows you to customize the behavior of the application.

![alt text](tuto-images/pluginBuilder/settings-ui.png "Settings UI")

- **General Tab**:
  - **Plugin-dist**: Set the output folder where built plugins will be stored.  
  - **Target Platform**: Choose target platforms (Win64, Mac, Linux, etc.). Note: only the host platform is officially supported for now.  
  - **Notifications**: Enable or disable event notifications (for example, build completion).  
- **UI Theme Tab**: Customize the look of the application (feature in development).

### 5. About Window (`AboutUs`)

This window displays information about the **Mecanes** team, including links to Discord and the Unreal Marketplace page.


<!-- ## 🗺️ Roadmap

- [ ] Parallel build support  
- [ ] Build log history  
- [ ] Automatic ZIP export  
- [ ] Custom build presets -->

## 📄 License

This project is distributed under the **MIT License**. See the `LICENSE` file for more details.

## 📧 Contact

Github : [PluginBuilder](https://github.com/Mecanes-Org/PluginBuilder)
