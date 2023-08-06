# Terra
*Terra* is a game engine made using C, which uses its own rendering engine made with *Vulkan*.

The libraries currently included in *Terra*, as well as the location of its associated headers are the following:
 - `terra_render (terrar/*)`: Code for the rendering engine. This wraps *Vulkan* and provides a higher level API, aiming to act as a high performance engine.
 - `terra_utils (terrau/*)`: Utilities, both for development and client use. Those that are used for development are hidden, so the `terrau` directory only contains headers that the clients can use.
