# radiant_slice

A vertical slice of a tactical FPS in Unreal Engine 5 — the Valorant/CS movement and
gunplay model, written in C++ rather than assembled from Blueprints.

The interesting part of a shooter is not the shooting; it is that the client and the
server disagree about where everyone is, and the game has to feel instant on a
connection where it cannot be. This is an attempt at the systems that paper over that.

> **Status: barebones slice.** These are the mechanics, not a game. There is no level,
> no HUD, no input bindings, no character mesh, and no `.uproject` — `Source/` is
> C++ against the UE5 API, meant to be dropped into a project. See
> [What isn't here](#what-isnt-here).

## The systems

### Counter-strafing (`Character/TacticalMovementComponent.cpp`)

Tactical shooters are only accurate standing still, so stopping fast is the core skill.
A custom `UCharacterMovementComponent` takes the dot product of current velocity against
current input each tick:

* Input opposing momentum (dot < -0.2) → friction spikes to `CounterStrafeFriction`, so
  tapping the opposite key stops you nearly dead.
* Input roughly with momentum → friction drops to zero, so strafing never scrubs speed.
* No input at all → normal friction, a natural slide to a stop.

Air control is clamped to 0.1 and acceleration set to 4000 for snappy grounded starts,
which is the tactical-shooter feel rather than the arena-shooter one.

### Deterministic recoil (`Combat/HitscanRifle.cpp`)

The spray pattern is a fixed `TArray<FVector2D>` of per-shot camera offsets — three shots
pulling down, then a sweep right — so the pattern is *learnable*. Random spread is layered
on top and scaled by movement speed: standing still is a 1.0x multiplier, running is up to
6x, which is what enforces stop-then-shoot without a hard accuracy rule.

### Lag compensation (`Combat/LagCompensationComponent.cpp`)

The server records each character's head position every tick into a doubly linked list,
trimmed to a 1-second rewind window. When a client reports a shot, `GetHistoricalFrame`
walks back to the two frames bracketing the client's timestamp and lerps between them,
reconstructing where the target *was* on the shooter's screen rather than where it is now
on the server.

This is why you can hit a target that has already run behind cover on your opponent's
machine, and it is the reason to keep history server-side at all.

### Server authority (`Combat/HitscanRifle.cpp`)

Firing is a client prediction plus a `Server_ConfirmHit` RPC (`Server, Reliable,
WithValidation`). Damage is applied only on the authority. The client's trace is
cosmetic.

## Layout

```
Source/Character/RadiantCharacter.*          pawn, first-person camera, head socket
Source/Character/TacticalMovementComponent.* counter-strafe friction model
Source/Combat/RadiantWeaponBase.*            ammo, fire rate, reload
Source/Combat/HitscanRifle.*                 recoil curve, spread, server RPC
Source/Combat/LagCompensationComponent.*     per-tick history and rewind
Source/Abilities/                            ability base + NearSight, DarkCover
Source/Core/                                 game mode, game state, player controller
```

## What isn't here

Honest accounting of the gap between the README and the code:

* **The rewind is not wired into hit validation.** `LagCompensationComponent` records and
  interpolates correctly, but `Server_ConfirmHit_Implementation` applies damage to the
  client-reported target directly instead of calling `GetHistoricalFrame` and re-tracing
  against the rewound position. That connection is the actual point and it is the next
  thing to build.
* **`HeadshotMultiplier` is declared and never applied.** Damage is flat `BaseDamage`,
  even though head position is exactly what the history tracks.
* **`Server_ConfirmHit_Validate` returns `true` unconditionally.** A real build has to
  bound the client's timestamp against the rewind window and sanity-check the trace
  length, or a client can claim any hit at any time.
* **No input bindings.** `SetupPlayerInputComponent` is an empty hook; Enhanced Input
  actions have to be set up in the editor.
* **No project files.** No `.uproject`, `.Build.cs`, content, maps, or meshes.
* **Abilities are stubs.** `Ability_NearSight` and `Ability_DarkCover` have structure but
  no gameplay effect.

## Author

**Abir Deol**
