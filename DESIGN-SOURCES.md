# Operating-system source roles

Cheap Phone OS is not based ideologically on one existing operating system. Use each source family for the problems it is unusually good at solving, and keep mechanism provenance separate from our interface/policy decisions.

| Source family | Primary role here | What not to infer |
| --- | --- | --- |
| Linux | Initial hardware implementation donor: SC9863A drivers, memory management, scheduler, filesystems, networking and broad peripheral support | Linux/Unix names, process APIs, policy and userspace architecture are not automatically our interface |
| NetBSD | Portability and device architecture: machine-independent vs machine-dependent separation, autoconfiguration, clean port boundaries | NetBSD need not become the hardware substrate merely because its organization is useful |
| FreeBSD | Mature VM, storage, networking and capability/jail ideas | Its complete userspace or compatibility surface is not a product requirement |
| OpenBSD | Simplification, auditable interfaces, defensive defaults, pledge/unveil-style authority reduction | Security mechanisms are design donors, not mandatory API compatibility |
| xv6 | Kernel comprehensibility: small examples of processes, VM, traps, scheduling, files and pipes | It does not contain the phone-specific hardware support needed to bring up the MIRO efficiently |
| Xinu | Very small kernel mechanisms and direct device-oriented design | Tiny-kernel structure alone does not justify rewriting working phone drivers |
| Zephyr | Embedded device model, configuration, power-aware design and real-time mechanisms | Phone-scale filesystem/process needs should not be forced into an RTOS architecture |
| NuttX | Small POSIX-like embedded system and configurable device support | POSIX compatibility remains optional for our user-facing design |
| EGOS / similarly small teaching systems | Layering and aggressively small understandable implementations | Teaching-system completeness is not physical-phone hardware completeness |
| Plan 9 | Namespaces and uniform resource interfaces; useful pressure toward making resources inspectable and composable | “Everything is a file” is a design question, not a rule to apply mechanically |
| MINIX | Driver/service separation and fault-containment ideas | Microkernel structure is not assumed to be worth a wholesale rewrite of Linux hardware support |
| Android / AOSP | Reference implementation and compatibility donor for phone-scale userspace mechanisms where useful | AOSP is not the required architecture of Cheap Phone OS |
| UNISOC/vendor BSP | Primary evidence for SC9863A/MIRO wiring, boot flow, firmware contracts, device trees, pin maps and vendor-specific phone mechanisms | Vendor interfaces and naming are evidence, not automatically desirable public APIs |

## Working rule

The initial implementation strategy is:

```text
small/clear OS ideas
        +
BSD portability/device lessons
        +
our filesystem/process/interface design
        ↓
Linux hardware mechanisms
        ↓
SC9863A drivers
        ↓
MIRO board description
        ↓
physical phone
```

Android and the UNISOC BSP sit alongside that stack as an evidence source for the physical phone and as donors for mechanisms that would otherwise need to be rediscovered.

Do not throw away a good existing mechanism for novelty. Do not preserve a bad interface merely because the mechanism underneath came from Linux, BSD, Unix, Android or a vendor tree.
