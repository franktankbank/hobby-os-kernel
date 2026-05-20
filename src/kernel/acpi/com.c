// #include "acpi_bus.h"
// #include "../console.h"

// #define COM_PNP_ID "PNP0501"

// static const char *const com_pnp_ids[] = {
//     COM_PNP_ID,
//     NULL
// };

// static int com_probe(uacpi_namespace_node *node, uacpi_namespace_node_info *info) {
//     uacpi_resources *com_res;

//     /* Parse the resources to find the IRQ and IO ports the keyboard is connected to
//      *
//      * Note that for a centralized system like that the resources could be passed
//      * to the device probe callback from common enumeration code at this point as
//      * well!
//      */
//     uacpi_status st = uacpi_get_current_resources(node, &com_res);
//     if (uacpi_unlikely_error(st)) {
//         Console.printf("unable to retrieve COM resources: %s", uacpi_status_to_string(st));
//         return -19;
//     }

//     // Actually instantiate the device
//     int ret =

//     uacpi_free_resources(com_res);
//     return ret;
// }
