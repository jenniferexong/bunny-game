#pragma once

namespace AttributeLocation {
	enum Location {
		Position = 0, Normal = 1, Texture = 2, ModelMatrixColumn1 = 3, ModelMatrixColumn2 = 4,
		ModelMatrixColumn3 = 5, ModelMatrixColumn4 = 6, ModelColor = 7
	};
}

namespace TextureLocation {
	enum Location { Base = 0, Red = 1, Green = 2, Blue = 3, BlendMap = 4 };
}

namespace CubeMapLocation {
	enum Location { Day = 0, Night = 1 };
}
