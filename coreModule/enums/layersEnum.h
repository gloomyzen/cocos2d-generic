#ifndef MERCENARY_BATTLES_LAYERSENUM_H
#define MERCENARY_BATTLES_LAYERSENUM_H

namespace mb {

	namespace coreModule {

		enum eGameLayers {
			ROOT = 0,
			SKY = 2,
			PLAN_3 = 4,
			PLAN_2 = 6,
			PLAN_1 = 8,
			FLOOR = 10,

			WINDOW = 20,
			DEBUG_LAYER = 99
		};
	}
}//mb::coreModule

#endif //MERCENARY_BATTLES_LAYERSENUM_H
