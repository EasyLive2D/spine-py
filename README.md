# spine-py

preview:

![preview](./doc/2025-03-21%2017-43-03%2000_00_08-00_00_13.gif)

usage:
```python
import spine
import pygame
import os
import time

cd = os.path.split(__file__)[0]

win_size = (400, 500)
pygame.display.set_mode(win_size, pygame.OPENGL | pygame.DOUBLEBUF | pygame.RESIZABLE)
spine.glInit()

atlas = spine.Atlas(os.path.join(cd, "../data/mix-and-match/mix-and-match-pma.atlas"))
skeletonJson = spine.SkeletonJson(atlas)
skeletonData = skeletonJson.readSkeletonDataFile(os.path.join(cd, "../data/mix-and-match/mix-and-match-pro.json"))
animationStateData = spine.AnimationStateData(skeletonData)
skeleton = spine.Skeleton(skeletonData, animationStateData)

skin = skeletonData.findSkin("full-skins/girl-spring-dress")
skeleton.setSkin(skin)
skeleton.setToSetupPose()

skeleton.usePremultipliedAlpha = True

skeleton.update(0)
skeleton.setAnimationByName(0, "idle", 0)
skeleton.addAnimationByName(0, "dance", True, 0)

skeleton.x = win_size[0] / 2
skeleton.y = win_size[1]
skeleton.canvasWidth = win_size[0]
skeleton.canvasHeight = win_size[1]

lastFrame = time.time()
currentFrame = time.time()
running = True
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
            break
    
    if not running:
        break

    spine.clearBuffer(g = 1.0)
    
    currentFrame = time.time()
    delta = currentFrame - lastFrame
    lastFrame = currentFrame

    size = pygame.display.get_window_size()
    spine.viewport(0, 0, size[0], size[1])
    skeleton.update(delta)
    skeleton.draw()

    pygame.display.flip()

pygame.quit()
```


available bindings:

```python
def glInit() -> None:
    ...

def clearBuffer(r: float = 0.0, g: float = 0.0, b: float = 0.0, a: float = 0.0) -> None:
    ...

class Atlas:
    
    def __init__(self, path: str): ...

class SkeletonJson:
    def __init__(self, atlas: Atlas): ...

    def readSkeletonDataFile(self, path: str) -> SkeletonData: ...

class Skin: ...

class SkeletonData:

    def findSkin(self, skeletonData: SkeletonData) -> Skin: ...

class AnimationStateData: ...

class Skeleton:

    def __init__(self, skeletonData: SkeletonData, animationStateData: AnimationStateData):
        self.usePremultipliedAlpha: bool = ...
        self.x: float = ...
        self.y: float = ...
        self.canvasWidth: int = ...
        self.canvasHeight: int = ...
    
    def update(self, deltaTime: float) -> None: ...
    
    def draw(self) -> None: ...
    
    def setSkin(self, skin: Skin) -> None: ...

    def setToSetupPose(self) -> None: ...

    def addAnimationByName(self, trackIndex: int, name: str, loop: bool, delay: float): ...

    def setAnimationByName(self, trackIndex: int, name: str, loop: bool): ...
```