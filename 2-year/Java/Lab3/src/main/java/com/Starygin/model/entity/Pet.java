package com.Starygin.model.entity;

import jakarta.persistence.*;
import org.hibernate.annotations.Fetch;
import org.hibernate.annotations.FetchMode;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = "pets", schema = "public")
public class Pet {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "pet_id")
    private Long id;

    @Column(name = "name")
    private String name;

    @Column(name = "birthday")
    private LocalDate birthday;

    @Enumerated(EnumType.STRING)
    @Column(name = "color")
    private PET_COLOR color;

    @Column(name = "tail_length")
    private double tailLength;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "owner_id", foreignKey = @ForeignKey(name = "fk_pet_owner"))
    private Owner owner;

    @ManyToMany
    @JoinTable(
            name = "pet_friends",
            joinColumns = @JoinColumn(name = "pet_id"),
            inverseJoinColumns = @JoinColumn(name = "friend_id"),
            foreignKey = @ForeignKey(name = "fk_pet_pet"),
            inverseForeignKey = @ForeignKey(name = "fk_friend_pet")
    )
    @Fetch(FetchMode.SUBSELECT)
    private List<Pet> friends = new ArrayList<>();

    public Pet(String name, LocalDate birthday, PET_COLOR color, double tailLength, Owner owner) {
        this.name = name;
        this.birthday = birthday;
        this.color = color;
        this.tailLength = tailLength;
        this.owner = owner;
    }

    public Pet() {

    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public LocalDate getBirthday() {
        return birthday;
    }

    public void setBirthday(LocalDate birthday) {
        this.birthday = birthday;
    }

    public PET_COLOR getColor() {
        return color;
    }

    public void setColor(PET_COLOR color) {
        this.color = color;
    }

    public double getTailLength() {
        return tailLength;
    }

    public void setTailLength(double tailLength) {
        this.tailLength = tailLength;
    }

    public Owner getOwner() {
        return owner;
    }

    public void setOwner(Owner ownerId) {
        this.owner = ownerId;
    }

    public List<Pet> getFriends() {
        return friends;
    }

    public void setFriends(List<Pet> friendId) {
        this.friends = friendId;
    }

    public void addFriend(Pet pet) {
        this.friends.add(pet);
    }
}
