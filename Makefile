.PHONY: all clean

all:
	@$(MAKE) -C MathEngine
	@$(MAKE) -C RenderEngine
	@$(MAKE) -C GameTest

clean:
	@$(MAKE) -C MathEngine clean
	@$(MAKE) -C RenderEngine clean
	@$(MAKE) -C GameTest clean