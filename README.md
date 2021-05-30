# SpotlightScreenSaver

A basic windows screensaver. Displays a digital clock centered across the virtual desktop. If you only have one monitor you have nothing to worry about, but those with two or more may notice oddities.

The desktop becomes black and parts are "illuminated" by a randomly moving light spot

# References

1. [Microsoft Documentation - Handling Screen Savers](https://docs.microsoft.com/en-us/windows/win32/lwef/screen-saver-library).
2. [Microsoft Documentation - Screen Saver command-line arguments](https://docs.microsoft.com/en-us/troubleshoot/windows/win32/screen-saver-command-line).
3. [Writing a Win32 Screen Saver](http://esaro.se/gamla/savers/sshow.html).
4. [Wikipedia - After Dark software](https://en.wikipedia.org/wiki/After_Dark_(software)).

# Leaflet.themeControls

A set of [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page") that extend Leaflet native controls, and other additional controls, in order to customize the on-screen shape of the control as defined by the underlying css and image files provided.

Using [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page") within a web map will enhance the user interface with a theme of cultural related icons.

<div><a href="https://essamatefelsherif.github.io/Leaflet.themeControls/demo-theme-egy.html" title="Ancient Egypt Theme"><img src="img/demo_egy.png"></a></div>

<div><a href="https://essamatefelsherif.github.io/Leaflet.themeControls/demo-theme-alx.html" title="Alexandria City Theme"><img src="img/demo_alx.png"></a></div>

## Demo

[Demo main page](https://essamatefelsherif.github.io/Leaflet.themeControls/):

1. [demo_1](https://essamatefelsherif.github.io/Leaflet.themeControls/demo-theme-egy.html) demonstrates using [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page") to provide a user interface relevant to *Ancient Egypt*.
2. [demo_2](https://essamatefelsherif.github.io/Leaflet.themeControls/demo-theme-alx.html) demonstrates using [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page") to provide a user interface relevant to the *Alexandria City of Egypt*.

## Control Class Hierarchy

| Class / Constructor                   | Factory function            | Description            | 
|---------------------------------------|-----------------------------|:----------------------:|
| `L.Control`                           |                             |                        |
| >>> `L.Control.Zoom`                  | `L.control.zoom`            | Leaflet native control |
| >>>>>> `L.ThemeZoomControl`           | `L.themeZoomControl`        | **themeControl**       |
| >>> `L.Control.Layers`                | `L.control.layers`          | Leaflet native control |
| >>>>>> `L.ThemeLayersControl`         | `L.themeLayersControl`      | **themeControl**       |
| >>> `L.Control.Attribution`           | `L.control.attribution`     | Leaflet native control |
| >>>>>> `L.ThemeAttributionControl`    | `L.themeAttributionControl` | **themeControl**       |
| >>> `L.Control.Screen`                | `L.control.screen`          | Additional control     |
| >>>>>> `L.ThemeScreenControl`         | `L.themeScreenControl`      | **themeControl**       |
| >>> `L.Control.Bookmark`              | `L.control.bookmark`        | Additional control     |
| >>>>>> `L.ThemeBookmarkControl`       | `L.themeBookmarkControl`    | **themeControl**       |
| >>> `L.Control.MessageBox`            | `L.control.messageBox`      | Additional control     |
| >>>>>> `L.ThemeMessageBox`            | `L.themeMessageBox`         | **themeControl**       |
| >>> `L.Control.Theme`                 | `L.control.theme`           | **themeControl**       |

## Usage

In order to use this plugin, include the essential [JS](src/js/theme-leaflet-controls.js) and [CSS](src/css/theme-egy-leaflet-controls.css) files in your HTML page as follows:

```
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="https://unpkg.com/leaflet@1.7.1/dist/leaflet.css" />
        <link rel="stylesheet" type="text/css" href="src/css/theme-egy-leaflet-controls.css" />

        <script type="text/javascript" src="https://unpkg.com/leaflet@1.7.1/dist/leaflet.js"></script>
        <script type="text/javascript" src="src/theme-leaflet-controls.js"></script>
    </head>
```

Each theme will be associated with a separate CSS file and linked image files. In order to build up your theme:

1. Prepare the set of image (png) files that will be linked to each of the [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page").
2. Assign a name to your theme, preferably 3 or 4 letters, example **'xyz'**.
3. Rename the given CSS file to reflect your theme **theme-xyz-leaflet-controls.css**.
4. Within **theme-xyz-leaflet-controls.css** file, replace any existence of my Ancient Egypt theme **'egy'** with yours **'xyz'**.
5. Alter the links to the image files in order to link to your files.
6. Include your CSS file in the HTML file as shown above.
7. Use the new CSS class names to create the [themeControls](https://essamatefelsherif.github.io/Leaflet.themeControls/ "Demo Page") within the \<script> section of your HTML file.

## Supported Leaflet Versions

This plugin was tested against Leaflet versions greater than 1.0, up to the latest 1.7.1.

## License

This plugin is licensed under the GPL license, see the LICENSE file.

