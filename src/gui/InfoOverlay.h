#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../misc/Printable.h"
#include "../misc/StringUtils.h"
#include <glm/glm.hpp>

namespace GUI {
	namespace Debug {
		void showOverlay(bool* open);
		void showOverlay(bool* open, std::vector<Printable*>* props);

		template <typename T>
		/*
		 * Holds a reference to a value with a label attached.
		 * Note that this is not a smart pointer, so destroying this 
		 * will not destroy the underlying value object, if it is one
		 */
		struct NamedValueItemReference : Printable {
			std::string name;
			const T* value;

			NamedValueItemReference(std::string name, const T* reference)
				: name(std::move(name)), value(reference) {

			}

			std::string toString() const {
				return name + ": " + StringUtils::toString(*value);
			}

			friend std::ostream& operator<<(std::ostream& stream, const GUI::Debug::NamedValueItemReference<T>& nvir) {
				return stream << nvir.toString();
			}
		};

		template <typename T>
		struct LabeledVec2 : Printable {
			std::string name;
			std::string labels[2];
			const T* values[2];

			LabeledVec2(std::string name, std::string xName, const T* xPtr, std::string yName, const T* yPtr)
				:name{ std::move(name) }, labels{ xName, yName }, values{ xPtr, yPtr }
			{ }

			std::string toString() const {
				return name + ": " + labels[0] + ": " + StringUtils::toString(*(values[0])) + ", " + labels[1] + ": " + StringUtils::toString(*(values[1]));
			}

			friend std::ostream& operator<<(std::ostream& stream, const GUI::Debug::LabeledVec2<T>& nvir) {
				return stream << nvir.toString();
			}
		};

		template <typename T>
		struct LabeledVec3 : Printable {
			std::string name;
			std::string labels[3];
			const T* values[3];
			const glm::vec3* vecPtr;

			LabeledVec3(std::string name, std::string xName, const T* xPtr, std::string yName, const T* yPtr, std::string zName, const T* zPtr)
				:name{ std::move(name) }, labels{ xName, yName, zName }, values{ xPtr, yPtr, zPtr }
			{ }

			LabeledVec3(std::string name, std::string xName, std::string yName, std::string zName, const glm::vec3* vec3)
				:name{ std::move(name) }, labels{ xName, yName, zName }, vecPtr{ vec3 }
			{ }

			std::string toString() const {
				std::ostringstream s;
				s << name << ": ";
				for (int i = 0; i < 3; i++) {
					s << labels[i] << ": ";
					if (vecPtr != nullptr) {
						s << (*vecPtr)[i];
					}
					else {
						s << *values[i];
					}
					if (i < 2) {
						s << ", ";
					}
				}
				return s.str();
			}

			friend std::ostream& operator<<(std::ostream& stream, const GUI::Debug::LabeledVec3<T>& nvir) {
				return stream << nvir.toString();
			}
		};
	}
}

