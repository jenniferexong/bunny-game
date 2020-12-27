#pragma once

namespace AttributeLocation {
	enum Location {
		Position = 0, Normal = 1, Texture = 2, ModelMatrixColumn1 = 3, ModelMatrixColumn2 = 4,
		ModelMatrixColumn3 = 5, ModelMatrixColumn4 = 6, ModelBrightness = 7
	};
}

namespace EntityTextureLocation {
	enum Location {
		DiffuseMap = 0, GlowMap = 1, EnvironmentMap = 2
	};
}

namespace TextAttributeLocation {
	enum Location {
		Position = 0, Texture = 1
	};
}

namespace TerrainAttributeLocation {
	enum Location {
		Position = 0, Normal = 1, Texture = 2, Tangent = 3
	};
}

namespace WaterTextureLocation {
	enum Location {
		Reflection = 0, Refraction = 1, DistortionMap = 2, NormalMap = 3, DepthMap = 4
	};
}

namespace TextureLocation {
	enum Location { Base = 0, Red = 1, Green = 2, Blue = 3, BlendMap = 4, NormalMap = 5 };
}

namespace CubeMapLocation {
	enum Location { Day = 0, Night = 1 };
}
