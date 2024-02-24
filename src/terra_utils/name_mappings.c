#include <terrar/vulkan.h>
#include <terrau/name_mappings.h>

const char *terrau_vk_format_name(VkFormat format) {
  switch (format) {
  case 0:
    return "VK_FORMAT_UNDEFINED";
    break;
  case 1:
    return "VK_FORMAT_R4G4_UNORM_PACK8";
    break;
  case 2:
    return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    break;
  case 3:
    return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    break;
  case 4:
    return "VK_FORMAT_R5G6B5_UNORM_PACK16";
    break;
  case 5:
    return "VK_FORMAT_B5G6R5_UNORM_PACK16";
    break;
  case 6:
    return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    break;
  case 7:
    return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    break;
  case 8:
    return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    break;
  case 9:
    return "VK_FORMAT_R8_UNORM";
    break;
  case 10:
    return "VK_FORMAT_R8_SNORM";
    break;
  case 11:
    return "VK_FORMAT_R8_USCALED";
    break;
  case 12:
    return "VK_FORMAT_R8_SSCALED";
    break;
  case 13:
    return "VK_FORMAT_R8_UINT";
    break;
  case 14:
    return "VK_FORMAT_R8_SINT";
    break;
  case 15:
    return "VK_FORMAT_R8_SRGB";
    break;
  case 16:
    return "VK_FORMAT_R8G8_UNORM";
    break;
  case 17:
    return "VK_FORMAT_R8G8_SNORM";
    break;
  case 18:
    return "VK_FORMAT_R8G8_USCALED";
    break;
  case 19:
    return "VK_FORMAT_R8G8_SSCALED";
    break;
  case 20:
    return "VK_FORMAT_R8G8_UINT";
    break;
  case 21:
    return "VK_FORMAT_R8G8_SINT";
    break;
  case 22:
    return "VK_FORMAT_R8G8_SRGB";
    break;
  case 23:
    return "VK_FORMAT_R8G8B8_UNORM";
    break;
  case 24:
    return "VK_FORMAT_R8G8B8_SNORM";
    break;
  case 25:
    return "VK_FORMAT_R8G8B8_USCALED";
    break;
  case 26:
    return "VK_FORMAT_R8G8B8_SSCALED";
    break;
  case 27:
    return "VK_FORMAT_R8G8B8_UINT";
    break;
  case 28:
    return "VK_FORMAT_R8G8B8_SINT";
    break;
  case 29:
    return "VK_FORMAT_R8G8B8_SRGB";
    break;
  case 30:
    return "VK_FORMAT_B8G8R8_UNORM";
    break;
  case 31:
    return "VK_FORMAT_B8G8R8_SNORM";
    break;
  case 32:
    return "VK_FORMAT_B8G8R8_USCALED";
    break;
  case 33:
    return "VK_FORMAT_B8G8R8_SSCALED";
    break;
  case 34:
    return "VK_FORMAT_B8G8R8_UINT";
    break;
  case 35:
    return "VK_FORMAT_B8G8R8_SINT";
    break;
  case 36:
    return "VK_FORMAT_B8G8R8_SRGB";
    break;
  case 37:
    return "VK_FORMAT_R8G8B8A8_UNORM";
    break;
  case 38:
    return "VK_FORMAT_R8G8B8A8_SNORM";
    break;
  case 39:
    return "VK_FORMAT_R8G8B8A8_USCALED";
    break;
  case 40:
    return "VK_FORMAT_R8G8B8A8_SSCALED";
    break;
  case 41:
    return "VK_FORMAT_R8G8B8A8_UINT";
    break;
  case 42:
    return "VK_FORMAT_R8G8B8A8_SINT";
    break;
  case 43:
    return "VK_FORMAT_R8G8B8A8_SRGB";
    break;
  case 44:
    return "VK_FORMAT_B8G8R8A8_UNORM";
    break;
  case 45:
    return "VK_FORMAT_B8G8R8A8_SNORM";
    break;
  case 46:
    return "VK_FORMAT_B8G8R8A8_USCALED";
    break;
  case 47:
    return "VK_FORMAT_B8G8R8A8_SSCALED";
    break;
  case 48:
    return "VK_FORMAT_B8G8R8A8_UINT";
    break;
  case 49:
    return "VK_FORMAT_B8G8R8A8_SINT";
    break;
  case 50:
    return "VK_FORMAT_B8G8R8A8_SRGB";
    break;
  case 51:
    return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
    break;
  case 52:
    return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
    break;
  case 53:
    return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
    break;
  case 54:
    return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
    break;
  case 55:
    return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
    break;
  case 56:
    return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
    break;
  case 57:
    return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
    break;
  case 58:
    return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    break;
  case 59:
    return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
    break;
  case 60:
    return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
    break;
  case 61:
    return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
    break;
  case 62:
    return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
    break;
  case 63:
    return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
    break;
  case 64:
    return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
    break;
  case 65:
    return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
    break;
  case 66:
    return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
    break;
  case 67:
    return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
    break;
  case 68:
    return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
    break;
  case 69:
    return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
    break;
  case 70:
    return "VK_FORMAT_R16_UNORM";
    break;
  case 71:
    return "VK_FORMAT_R16_SNORM";
    break;
  case 72:
    return "VK_FORMAT_R16_USCALED";
    break;
  case 73:
    return "VK_FORMAT_R16_SSCALED";
    break;
  case 74:
    return "VK_FORMAT_R16_UINT";
    break;
  case 75:
    return "VK_FORMAT_R16_SINT";
    break;
  case 76:
    return "VK_FORMAT_R16_SFLOAT";
    break;
  case 77:
    return "VK_FORMAT_R16G16_UNORM";
    break;
  case 78:
    return "VK_FORMAT_R16G16_SNORM";
    break;
  case 79:
    return "VK_FORMAT_R16G16_USCALED";
    break;
  case 80:
    return "VK_FORMAT_R16G16_SSCALED";
    break;
  case 81:
    return "VK_FORMAT_R16G16_UINT";
    break;
  case 82:
    return "VK_FORMAT_R16G16_SINT";
    break;
  case 83:
    return "VK_FORMAT_R16G16_SFLOAT";
    break;
  case 84:
    return "VK_FORMAT_R16G16B16_UNORM";
    break;
  case 85:
    return "VK_FORMAT_R16G16B16_SNORM";
    break;
  case 86:
    return "VK_FORMAT_R16G16B16_USCALED";
    break;
  case 87:
    return "VK_FORMAT_R16G16B16_SSCALED";
    break;
  case 88:
    return "VK_FORMAT_R16G16B16_UINT";
    break;
  case 89:
    return "VK_FORMAT_R16G16B16_SINT";
    break;
  case 90:
    return "VK_FORMAT_R16G16B16_SFLOAT";
    break;
  case 91:
    return "VK_FORMAT_R16G16B16A16_UNORM";
    break;
  case 92:
    return "VK_FORMAT_R16G16B16A16_SNORM";
    break;
  case 93:
    return "VK_FORMAT_R16G16B16A16_USCALED";
    break;
  case 94:
    return "VK_FORMAT_R16G16B16A16_SSCALED";
    break;
  case 95:
    return "VK_FORMAT_R16G16B16A16_UINT";
    break;
  case 96:
    return "VK_FORMAT_R16G16B16A16_SINT";
    break;
  case 97:
    return "VK_FORMAT_R16G16B16A16_SFLOAT";
    break;
  case 98:
    return "VK_FORMAT_R32_UINT";
    break;
  case 99:
    return "VK_FORMAT_R32_SINT";
    break;
  case 100:
    return "VK_FORMAT_R32_SFLOAT";
    break;
  case 101:
    return "VK_FORMAT_R32G32_UINT";
    break;
  case 102:
    return "VK_FORMAT_R32G32_SINT";
    break;
  case 103:
    return "VK_FORMAT_R32G32_SFLOAT";
    break;
  case 104:
    return "VK_FORMAT_R32G32B32_UINT";
    break;
  case 105:
    return "VK_FORMAT_R32G32B32_SINT";
    break;
  case 106:
    return "VK_FORMAT_R32G32B32_SFLOAT";
    break;
  case 107:
    return "VK_FORMAT_R32G32B32A32_UINT";
    break;
  case 108:
    return "VK_FORMAT_R32G32B32A32_SINT";
    break;
  case 109:
    return "VK_FORMAT_R32G32B32A32_SFLOAT";
    break;
  case 110:
    return "VK_FORMAT_R64_UINT";
    break;
  case 111:
    return "VK_FORMAT_R64_SINT";
    break;
  case 112:
    return "VK_FORMAT_R64_SFLOAT";
    break;
  case 113:
    return "VK_FORMAT_R64G64_UINT";
    break;
  case 114:
    return "VK_FORMAT_R64G64_SINT";
    break;
  case 115:
    return "VK_FORMAT_R64G64_SFLOAT";
    break;
  case 116:
    return "VK_FORMAT_R64G64B64_UINT";
    break;
  case 117:
    return "VK_FORMAT_R64G64B64_SINT";
    break;
  case 118:
    return "VK_FORMAT_R64G64B64_SFLOAT";
    break;
  case 119:
    return "VK_FORMAT_R64G64B64A64_UINT";
    break;
  case 120:
    return "VK_FORMAT_R64G64B64A64_SINT";
    break;
  case 121:
    return "VK_FORMAT_R64G64B64A64_SFLOAT";
    break;
  case 122:
    return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
    break;
  case 123:
    return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
    break;
  case 124:
    return "VK_FORMAT_D16_UNORM";
    break;
  case 125:
    return "VK_FORMAT_X8_D24_UNORM_PACK32";
    break;
  case 126:
    return "VK_FORMAT_D32_SFLOAT";
    break;
  case 127:
    return "VK_FORMAT_S8_UINT";
    break;
  case 128:
    return "VK_FORMAT_D16_UNORM_S8_UINT";
    break;
  case 129:
    return "VK_FORMAT_D24_UNORM_S8_UINT";
    break;
  case 130:
    return "VK_FORMAT_D32_SFLOAT_S8_UINT";
    break;
  case 131:
    return "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
    break;
  case 132:
    return "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
    break;
  case 133:
    return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
    break;
  case 134:
    return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
    break;
  case 135:
    return "VK_FORMAT_BC2_UNORM_BLOCK";
    break;
  case 136:
    return "VK_FORMAT_BC2_SRGB_BLOCK";
    break;
  case 137:
    return "VK_FORMAT_BC3_UNORM_BLOCK";
    break;
  case 138:
    return "VK_FORMAT_BC3_SRGB_BLOCK";
    break;
  case 139:
    return "VK_FORMAT_BC4_UNORM_BLOCK";
    break;
  case 140:
    return "VK_FORMAT_BC4_SNORM_BLOCK";
    break;
  case 141:
    return "VK_FORMAT_BC5_UNORM_BLOCK";
    break;
  case 142:
    return "VK_FORMAT_BC5_SNORM_BLOCK";
    break;
  case 143:
    return "VK_FORMAT_BC6H_UFLOAT_BLOCK";
    break;
  case 144:
    return "VK_FORMAT_BC6H_SFLOAT_BLOCK";
    break;
  case 145:
    return "VK_FORMAT_BC7_UNORM_BLOCK";
    break;
  case 146:
    return "VK_FORMAT_BC7_SRGB_BLOCK";
    break;
  case 147:
    return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
    break;
  case 148:
    return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
    break;
  case 149:
    return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
    break;
  case 150:
    return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
    break;
  case 151:
    return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
    break;
  case 152:
    return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
    break;
  case 153:
    return "VK_FORMAT_EAC_R11_UNORM_BLOCK";
    break;
  case 154:
    return "VK_FORMAT_EAC_R11_SNORM_BLOCK";
    break;
  case 155:
    return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
    break;
  case 156:
    return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
    break;
  case 157:
    return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
    break;
  case 158:
    return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
    break;
  case 159:
    return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
    break;
  case 160:
    return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
    break;
  case 161:
    return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
    break;
  case 162:
    return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
    break;
  case 163:
    return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
    break;
  case 164:
    return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
    break;
  case 165:
    return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
    break;
  case 166:
    return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
    break;
  case 167:
    return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
    break;
  case 168:
    return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
    break;
  case 169:
    return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
    break;
  case 170:
    return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
    break;
  case 171:
    return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
    break;
  case 172:
    return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
    break;
  case 173:
    return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
    break;
  case 174:
    return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
    break;
  case 175:
    return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
    break;
  case 176:
    return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
    break;
  case 177:
    return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
    break;
  case 178:
    return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
    break;
  case 179:
    return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
    break;
  case 180:
    return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
    break;
  case 181:
    return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
    break;
  case 182:
    return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
    break;
  case 183:
    return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
    break;
  case 184:
    return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
    break;
  case 1000156000:
    return "VK_FORMAT_G8B8G8R8_422_UNORM";
    break;
  case 1000156001:
    return "VK_FORMAT_B8G8R8G8_422_UNORM";
    break;
  case 1000156002:
    return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM";
    break;
  case 1000156003:
    return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM";
    break;
  case 1000156004:
    return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM";
    break;
  case 1000156005:
    return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM";
    break;
  case 1000156006:
    return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM";
    break;
  case 1000156007:
    return "VK_FORMAT_R10X6_UNORM_PACK16";
    break;
  case 1000156008:
    return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16";
    break;
  case 1000156009:
    return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16";
    break;
  case 1000156010:
    return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16";
    break;
  case 1000156011:
    return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16";
    break;
  case 1000156012:
    return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16";
    break;
  case 1000156013:
    return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16";
    break;
  case 1000156014:
    return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16";
    break;
  case 1000156015:
    return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16";
    break;
  case 1000156016:
    return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16";
    break;
  case 1000156017:
    return "VK_FORMAT_R12X4_UNORM_PACK16";
    break;
  case 1000156018:
    return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16";
    break;
  case 1000156019:
    return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16";
    break;
  case 1000156020:
    return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16";
    break;
  case 1000156021:
    return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16";
    break;
  case 1000156022:
    return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16";
    break;
  case 1000156023:
    return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16";
    break;
  case 1000156024:
    return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16";
    break;
  case 1000156025:
    return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16";
    break;
  case 1000156026:
    return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16";
    break;
  case 1000156027:
    return "VK_FORMAT_G16B16G16R16_422_UNORM";
    break;
  case 1000156028:
    return "VK_FORMAT_B16G16R16G16_422_UNORM";
    break;
  case 1000156029:
    return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM";
    break;
  case 1000156030:
    return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM";
    break;
  case 1000156031:
    return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM";
    break;
  case 1000156032:
    return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM";
    break;
  case 1000156033:
    return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM";
    break;
  case 1000330000:
    return "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM";
    break;
  case 1000330001:
    return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16";
    break;
  case 1000330002:
    return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16";
    break;
  case 1000330003:
    return "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM";
    break;
  case 1000340000:
    return "VK_FORMAT_A4R4G4B4_UNORM_PACK16";
    break;
  case 1000340001:
    return "VK_FORMAT_A4B4G4R4_UNORM_PACK16";
    break;
  case 1000066000:
    return "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK";
    break;
  case 1000066001:
    return "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK";
    break;
  case 1000066002:
    return "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK";
    break;
  case 1000066003:
    return "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK";
    break;
  case 1000066004:
    return "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK";
    break;
  case 1000066005:
    return "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK";
    break;
  case 1000066006:
    return "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK";
    break;
  case 1000066007:
    return "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK";
    break;
  case 1000066008:
    return "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK";
    break;
  case 1000066009:
    return "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK";
    break;
  case 1000066010:
    return "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK";
    break;
  case 1000066011:
    return "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK";
    break;
  case 1000066012:
    return "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK";
    break;
  case 1000066013:
    return "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK";
    break;
  case 1000054000:
    return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
    break;
  case 1000054001:
    return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
    break;
  case 1000054002:
    return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
    break;
  case 1000054003:
    return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
    break;
  case 1000054004:
    return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
    break;
  case 1000054005:
    return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
    break;
  case 1000054006:
    return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
    break;
  case 1000054007:
    return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
    break;
  case 1000464000:
    return "VK_FORMAT_R16G16_S10_5_NV";
    break;
  case 1000470000:
    return "VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR";
    break;
  case 1000470001:
    return "VK_FORMAT_A8_UNORM_KHR";
    break;
  case 0x7FFFFFFF:
    return "VK_FORMAT_MAX_ENUM";
    break;
  default:
    return "Unknown";
    break;
  }
}

const char *terrau_vk_colorspace_name(VkColorSpaceKHR color_space) {
  switch (color_space) {
  case 0:
    return "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR";
    break;
  case 1000104001:
    return "VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT";
    break;
  case 1000104002:
    return "VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT";
    break;
  case 1000104003:
    return "VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT";
    break;
  case 1000104004:
    return "VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT";
    break;
  case 1000104005:
    return "VK_COLOR_SPACE_BT709_LINEAR_EXT";
    break;
  case 1000104006:
    return "VK_COLOR_SPACE_BT709_NONLINEAR_EXT";
    break;
  case 1000104007:
    return "VK_COLOR_SPACE_BT2020_LINEAR_EXT";
    break;
  case 1000104008:
    return "VK_COLOR_SPACE_HDR10_ST2084_EXT";
    break;
  case 1000104009:
    return "VK_COLOR_SPACE_DOLBYVISION_EXT";
    break;
  case 1000104010:
    return "VK_COLOR_SPACE_HDR10_HLG_EXT";
    break;
  case 1000104011:
    return "VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT";
    break;
  case 1000104012:
    return "VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT";
    break;
  case 1000104013:
    return "VK_COLOR_SPACE_PASS_THROUGH_EXT";
    break;
  case 1000104014:
    return "VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT";
    break;
  case 1000213000:
    return "VK_COLOR_SPACE_DISPLAY_NATIVE_AMD";
    break;
  case 0x7FFFFFFF:
    return "VK_COLOR_SPACE_MAX_ENUM_KHR";
    break;
  default:
    return "Unknown";
    break;
  }
}
